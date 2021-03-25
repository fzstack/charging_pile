#pragma once
#include "hlw8112.hxx"
#include <rtthread.h>

extern "C" {
#include <drv_spi.h>
}

class SpiHlw8112: public Hlw8112 {
public:
    SpiHlw8112(const char* spiBus, const char* spiDev, rt_base_t ssPin);

protected:
    virtual void cmd(int cmd, void* data, int len) override;
    virtual void readReg(char addr, void* data, int len, rt_int32_t timeout = 5) override;

private:
    void reverseCopy(rt_uint8_t* dst, const rt_uint8_t* src, int len);

private:
    rt_spi_device* spiDev;
};

#include <utilities/singleton.hxx>
#include "uart_hlw8112.hxx"
#include <utilities/preset.hxx>
namespace Preset {
template<int R>
class SpiHlw8112: public Singleton<SpiHlw8112<R>>, public ::SpiHlw8112 {
    friend class Singleton<SpiHlw8112<R>>;
    SpiHlw8112(): ::SpiHlw8112(kSpiBus, kSpiDev, kSsPin){}
    static const char *kSpiBus;
    static const char *kSpiDev;
    static const rt_base_t kSsPin;
};

template<>
struct Trait<::SpiHlw8112> {
    static constexpr int Num = 5;
};
}
