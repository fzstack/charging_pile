/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-12-10     imgcr       the first version
 */

#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>
#include <map>
#include <functional>
#include <memory>
#include <optional>

#include "rc522.hxx"

#define LOG_TAG "dev.rc522"

using namespace std;

Rc522::Rc522(const char* spiBus, const char* spiDev, const GPIO_TypeDef* ssGpioX, const rt_uint16_t ssGpioPin): oCardId(cardId) {
    rt_hw_spi_device_attach(spiBus, spiDev, (GPIO_TypeDef*)ssGpioX, ssGpioPin);

    spi_dev = (rt_spi_device*)rt_device_find(spiDev);
    rt_spi_configuration conf;

    conf.data_width = 8;
    conf.mode = RT_SPI_MASTER | RT_SPI_MODE_3 | RT_SPI_MSB;
    conf.max_hz = 1 * 1000 * 1000;
    rt_spi_configure(spi_dev, &conf);

    inited.onChanged += [this](auto value) {
        if(value) {
            timer = std::shared_ptr<rt_timer>(rt_timer_create(LOG_TAG, [](auto p) {
                auto self = (Rc522*)p;
                auto retVal = self->pcdRequest(Rc522::Piccs::ReqAll);
                if(retVal != 0 && retVal != 8) { //值未知，发生错误，需要重新复位
                    self->pcdReset();
                    return;
                }
                if(retVal != RT_EOK) {
                    self->contiReqFailedCnt++;
                    if(self->contiReqFailedCnt > 1)
                        self->cardId = nullopt;
                    return;
                }
                self->contiReqFailedCnt = 0;
                auto acRetVal = self->pcdAntiColl();
                auto pStr = std::get_if<std::string>(&acRetVal);
                if(pStr) {
                    self->cardId = *pStr;
                } else {
                    self->cardId = nullopt;
                }
            }, this, kDetectDurMs, RT_TIMER_FLAG_PERIODIC | RT_TIMER_FLAG_SOFT_TIMER), [](auto p) {
                rt_timer_stop(p);
                rt_timer_delete(p);
            });
            pcdReset();
            rt_timer_start(timer.get());
        } else {
            timer = nullptr;
        }
    };
}

void Rc522::init() {
    inited = true;
}

void Rc522::writeReg(rt_uint8_t addr, rt_uint8_t value) {
    rt_uint8_t data[] = {rt_uint8_t((addr << 1) & ~(1 << 7)), value};
    rt_spi_send(spi_dev, data, sizeof(data));
}

rt_uint8_t Rc522::readReg(rt_uint8_t addr) {
    rt_uint8_t data[] = {rt_uint8_t((addr << 1) | (1 << 7))};
    rt_uint8_t recv_byte;
    rt_spi_send_then_recv(spi_dev, data, sizeof(data), &recv_byte, sizeof(recv_byte));
    return recv_byte;
}

void Rc522::pcdReset() {
    (*this)[Regs::Command] = 0x0f;
    while((*this)[Regs::Command] & 0x10);
    (*this)[Regs::Mode] = 0x3d;
    (*this)[Regs::TReloadL] = 30;
    (*this)[Regs::TReloadH] = 0;
    (*this)[Regs::TMode] = 0x8d;
    (*this)[Regs::TPrescaler] = 0x3e;
    (*this)[Regs::TxAuto] = 0x40;
    pcdConfigISOType();
}

void Rc522::pcdConfigISOType(char type) {
    auto sel = map<char, function<void()>> {
        {'A', [this](){
            (*this)[Regs::Status2] &= ~0x08;
            (*this)[Regs::Mode] = 0x3d;
            (*this)[Regs::RxSel] = 0x86;
            (*this)[Regs::RFCfg] = 0x7f;
            (*this)[Regs::TReloadL] = 30;
            (*this)[Regs::TReloadH] = 0;
            (*this)[Regs::TMode] = 0x8d;
            (*this)[Regs::TPrescaler] = 0x3e;
            pcdAntennaOn();
        }},
    };
    auto found = sel.find(type);
    if(found != sel.end()) {
        found->second();
    }
}

void Rc522::pcdAntennaOn() {
    auto data = (*this)[Regs::TxControl];
    if(!(data & 0x03)) {
        (*this)[Regs::TxControl] |= 0x03;
    }
}

rt_err_t Rc522::pcdRequest(Rc522::Piccs::Value reqCode) {
    (*this)[Regs::Status2] &= ~0x08;
    (*this)[Regs::BitFraming] = 0x07;
    (*this)[Regs::TxControl] |= 0x03;
    auto [err, _] = pcdComMF522(PcdCmds::Transceive, {reqCode});
    return err;
}

auto Rc522::pcdComMF522(PcdCmds::Value cmd, std::vector<rt_uint8_t>&& in)
-> std::tuple<rt_err_t, std::optional<std::pair<std::vector<rt_uint8_t>, int>>> {
    auto irqEn = rt_uint8_t{};
    auto waitFor = rt_uint8_t{};
    switch(cmd) {
    case PcdCmds::Authent:
        irqEn = 0x12;
        waitFor = 0x10;
        break;
    case PcdCmds::Transceive:
        irqEn = 0x77;
        waitFor = 0x30;
        break;
    default:
        break;
    }
    (*this)[Regs::ComIEn] = irqEn | 0x80;
    (*this)[Regs::ComIrq] &= ~0x80;
    (*this)[Regs::Command] = PcdCmds::Idle;
    (*this)[Regs::FIFOLevel] |= 0x80;
    for(const auto data: in) {
        (*this)[Regs::FIFOData] = data;
    }
    (*this)[Regs::Command] = cmd;
    auto guard = std::shared_ptr<void>(nullptr, [this](auto p){
        (*this)[Regs::Control] |= 0x80;
        (*this)[Regs::Command] = PcdCmds::Idle;
    });
    if(cmd == PcdCmds::Transceive) {
        (*this)[Regs::BitFraming] |= 0x80;
    }
    auto count = 1000;
    auto irqVal = rt_uint8_t{};
    do {
        irqVal = (*this)[Regs::ComIrq];
        count--;
    } while((count != 0) && !(irqVal & 0x01) && !(irqVal & waitFor));
    (*this)[Regs::BitFraming] &= ~0x80;
    if(count == 0) {
        return {RT_ETIMEOUT, {}};
    }
    if((*this)[Regs::Error] & 0x1b) {
        return {RT_ERROR, {}};
    }
    auto retVal = RT_EOK;
    if(irqVal & irqEn & 0x01) {
        retVal = RT_EIO;
    }
    auto totalBits = int{};
    auto out = vector<rt_uint8_t>{};
    if(cmd == PcdCmds::Transceive) {
        auto n2Recv = (*this)[Regs::FIFOLevel];
        auto lastBits = (*this)[Regs::Control] & 0x07;
        totalBits = lastBits == 0 ? n2Recv * 8 : lastBits + (n2Recv - 1) * 8;
        if(n2Recv == 0)
            n2Recv = 1;
        out.resize(n2Recv);
        for(auto& data: out) {
            data = (*this)[Regs::FIFOData];
        }
    }
    return {retVal, {{out, totalBits}}};
}

auto Rc522::pcdAntiColl() -> std::variant<rt_err_t, std::string> {
    (*this)[Regs::Status2] &= ~0x08;
    (*this)[Regs::BitFraming] = 0x00;
    (*this)[Regs::Coll] &= ~0x80;
    auto guard = std::shared_ptr<void>(nullptr, [this](auto){
        (*this)[Regs::Coll] |= 0x80;
    });

    auto [err, info] = pcdComMF522(PcdCmds::Transceive, {0x93, 0x20});
    if(err != RT_EOK)
        return err;

    auto [data, _] = *info;
    if(data.size() < 5)
        return RT_ERROR;

    auto checksum = rt_uint8_t{};
    for(auto i = 0; i < 4; i++) {
        checksum ^= data[i];
    }

    if(checksum != data[4])
        return RT_ERROR;

    auto buff = std::shared_ptr<char[]>(new char[9]);
    sprintf(buff.get(), "%02x%02x%02x%02x", data[0], data[1], data[2], data[3]);

    return std::string(buff.get());
}

Rc522::Reg Rc522::operator[](Rc522::Regs::Value r) {
    return Rc522::Reg(this, r);
}

void Rc522::clearBitMask(rt_uint8_t addr, rt_uint8_t mask) {
    auto value = readReg(addr);
    writeReg(addr, value & ~mask);
}

void Rc522::setBitMask(rt_uint8_t addr, rt_uint8_t mask) {
    auto value = readReg(addr);
    writeReg(addr, value | mask);
}

Rc522::Reg::Reg(Rc522* owner, Regs::Value r): owner(owner), r(r) {

}

void Rc522::Reg::operator=(const rt_uint8_t& right) {
    owner->writeReg(r, right);
}


void Rc522::Reg::operator&=(const int right) {
    auto value = owner->readReg(r);
    owner->writeReg(r, value & rt_uint8_t(right));
}

void Rc522::Reg::operator|=(const rt_uint8_t right) {
    auto value = owner->readReg(r);
    owner->writeReg(r, value | right);
}

Rc522::Reg::operator rt_uint8_t() {
    return owner->readReg(r);
}
