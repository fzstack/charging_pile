/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-12-10     imgcr       the first version
 */
#ifndef DEVICES_RC522_HXX_
#define DEVICES_RC522_HXX_

#include <rtthread.h>
#include <vector>
#include <optional>
#include <tuple>
#include <string>
#include <variant>
#include <board.h>
#include <utilities/observer.hxx>
#include <optional>
#include <memory>

extern "C" {
#include <drv_spi.h>
}

class Rc522 {
public:
    Rc522(const char* spiBus, const char* spiDev, rt_base_t ssPin);
    void init();

private:
    static const int kDetectDurMs = 250;

    using card_id_t = std::optional<std::string>;
private:
    Observable<card_id_t> cardId;
public:
    Observer<card_id_t> oCardId;

private:
    Observable<bool> inited = {false};
    int contiReqFailedCnt;


private:
    void pcdReset();
    void pcdConfigISOType(char type = 'A');

    struct Regs {
        enum Value: rt_uint8_t {
            RFU00 = 0x00,
            Command = 0x01,
            ComIEn = 0x02,
            DivlEn = 0x03,
            ComIrq = 0x04,
            DivIrq = 0x05,
            Error = 0x06,
            Status1 = 0x07,
            Status2 = 0x08,
            FIFOData = 0x09,
            FIFOLevel = 0x0A,
            WaterLevel = 0x0B,
            Control = 0x0C,
            BitFraming = 0x0D,
            Coll = 0x0E,
            RFU0F = 0x0F,
            RFU10 = 0x10,
            Mode = 0x11,
            TxMode = 0x12,
            RxMode = 0x13,
            TxControl = 0x14,
            TxAuto = 0x15,
            TxSel = 0x16,
            RxSel = 0x17,
            RxThreshold = 0x18,
            Demod = 0x19,
            RFU1A = 0x1A,
            RFU1B = 0x1B,
            Mifare = 0x1C,
            RFU1D = 0x1D,
            RFU1E = 0x1E,
            SerialSpeed = 0x1F,
            RFU20 = 0x20,
            CRCResultM = 0x21,
            CRCResultL = 0x22,
            RFU23 = 0x23,
            ModWidth = 0x24,
            RFU25 = 0x25,
            RFCfg = 0x26,
            GsN = 0x27,
            CWGsCfg = 0x28,
            ModGsCfg = 0x29,
            TMode = 0x2A,
            TPrescaler = 0x2B,
            TReloadH = 0x2C,
            TReloadL = 0x2D,
            TCounterValueH = 0x2E,
            TCounterValueL = 0x2F,
            RFU30 = 0x30,
            TestSel1 = 0x31,
            TestSel2 = 0x32,
            TestPinEn = 0x33,
            TestPinValue = 0x34,
            TestBus = 0x35,
            AutoTest = 0x36,
            Version = 0x37,
            AnalogTest = 0x38,
            TestDAC1 = 0x39,
            TestDAC2 = 0x3A,
            TestADC = 0x3B,
            RFU3C = 0x3C,
            RFU3D = 0x3D,
            RFU3E = 0x3E,
            RFU3F = 0x3F,
        };
    };

    struct PcdCmds {
        enum Value: rt_uint8_t {
            Idle = 0x00,
            Authent = 0x0e,
            Receive = 0x08,
            Transmit = 0x04,
            Transceive = 0x0c,
            ResetPhase = 0x0f,
            CalcCRC = 0x03,
        };
    };

    struct Piccs {
        enum Value: rt_uint8_t {
            ReqIdl = 0x26,
            ReqAll = 0x52,
            AntiColl1 = 0x93,
            AntiColl2 = 0x95,
            Authent1A = 0x60,
            Authent1B = 0x61,
            Read = 0x30,
            Write = 0xa0,
            Decrement = 0xc0,
            Increment = 0xc1,
            Restore = 0xc2,
            Transfer = 0xb0,
            Halt = 0x50,
        };
    };

    class Reg {
        friend class Rc522;
    public:
        void operator=(const rt_uint8_t& right);
        void operator|=(const rt_uint8_t right);
        void operator&=(const int right);
        operator rt_uint8_t();


    private:
        Reg(Rc522* owner, Regs::Value r);

        Rc522* owner;
        Regs::Value r;
    };

    Reg operator[](Regs::Value r);

private:
    void writeReg(rt_uint8_t addr, rt_uint8_t value);
    rt_uint8_t readReg(rt_uint8_t addr);
    void clearBitMask(rt_uint8_t addr, rt_uint8_t mask);
    void setBitMask(rt_uint8_t addr, rt_uint8_t mask);
    void pcdAntennaOn();

    std::variant<rt_err_t, std::string> pcdAntiColl();
    rt_err_t pcdRequest(Piccs::Value reqCode);
    auto pcdComMF522(PcdCmds::Value cmd, std::vector<rt_uint8_t>&& in) -> std::tuple<rt_err_t, std::optional<std::pair<std::vector<rt_uint8_t>, int>>>;
    rt_spi_device* spi_dev;
    std::shared_ptr<rt_timer> timer;
};

#include <utilities/singleton.hxx>
namespace Preset {
class Rc522: public Singleton<Rc522>, public ::Rc522 {
    friend class Singleton<Rc522>;
    Rc522(): ::Rc522(kSpiBus, kSpiDev, kSsPin) {}
    static const char *kSpiBus, *kSpiDev;
    static const rt_base_t kSsPin;
};
}


#endif /* DEVICES_RC522_HXX_ */
