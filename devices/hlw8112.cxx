/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-08-25     imgcr       the first version
 */

#include "hlw8112.hxx"

using namespace std;
using namespace Hlw8112ns::Regs;

Hlw8112::Hlw8112(const char* uartDeviceName) {
    struct serial_configure conf = RT_SERIAL_CONFIG_DEFAULT;
    conf.data_bits = DATA_BITS_9;
    conf.baud_rate = BAUD_RATE_9600;
    conf.parity = PARITY_EVEN;
    uart = make_shared<QueuedUart>(uartDeviceName, &conf);
    writable.onChanged += [this](auto value) {
      if(!value) return;
      if(*value) {
          writeEnable();
      } else {
          writeDisable();
      }
    };

}

void Hlw8112::writeEnable() {
    specCmd(0xe5);
}

void Hlw8112::writeDisable() {
    specCmd(0xdc);
}

void Hlw8112::init() {
    specCmd(0x96);
    writable = nullopt;
    writable = true;
    {
        auto sess = makeSess<emucon2>();
        sess->chs_ib = true;
        sess->dup_sel = emucon2::DupSel::f3_4Hz;
        sess->sdo_cmos = false;
    } {
        auto sess = makeSess<emucon>();
        sess->hpf_i_a_off = false; //关闭高通滤波器
        sess->hpf_i_b_off = false;
        sess->hpf_u_off = false;
        sess->comp_off = true;
    } {
        auto sess = makeSess<pin>();
        sess->p1_sel = pin::PSel::IRQ;
    } {
        auto sess = makeSess<ie>();
        sess->dupd = true;
    } {
        auto sess = makeSess<syscon>();
        sess->adc1_on = true;
        sess->pga_i_a = syscon::PGA::x16;
        sess->adc2_on = true;
        sess->pga_i_b = syscon::PGA::x16;
        sess->adc3_on = true;
        sess->pga_u = syscon::PGA::x1;
    }
    writable = false;
}

void Hlw8112::cmd(int cmd, void* data, int len) {
    char cs = 0;
    uart->send<char>(0xa5);
    cs += 0xa5;

    uart->send<char>(cmd);
    cs += cmd;

    for(char* p = (char*)data + len - 1; p >= (char*)data; p--) {
        uart->send(p, 1);
        cs += *p;
    }

    cs = ~cs;
    uart->send(cs);
}

void Hlw8112::specCmd(char cmd) {
    this->cmd(0xea, (void*)&cmd, 1);
}

rt_err_t Hlw8112::readReg(char addr, void* data, int len, rt_int32_t timeout) {
    char cs_expect = 0, cs;
    uart->send<char>(0xa5);
    cs_expect += 0xa5;

    uart->send(addr);
    cs_expect += addr;

    for(char* p = (char*)data + len - 1; p >= (char*)data; p--) {
        if(uart->recv(p, 1, timeout) != RT_EOK) {
            return -RT_ETIMEOUT;
        }
        cs_expect += *p;
    }
    cs_expect = ~cs_expect;
    if(uart->recv(&cs, 1, timeout) != RT_EOK) {
        return RT_ETIMEOUT;
    }

    if(cs != cs_expect) {
        return -RT_ERROR;
    }

    return RT_EOK;
}

void Hlw8112::writeReg(int addr, void* data, int len) {
    cmd(addr | 0x80, data, len);
}
