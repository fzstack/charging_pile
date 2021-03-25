#include "spi_hlw8112.hxx"
#include <rtdevice.h>
#include <vector>
#include "pin_map.hxx"

using namespace std;

SpiHlw8112::SpiHlw8112(const char* spiBus, const char* spiDev, rt_base_t ssPin) {
    rt_hw_spi_device_attach(spiBus, spiDev, pins[ssPin].gpio, pins[ssPin].pin);

    this->spiDev = (rt_spi_device*)rt_device_find(spiDev);
    rt_spi_configuration conf;

    conf.data_width = 8;
    conf.mode = RT_SPI_MASTER | RT_SPI_MODE_2 | RT_SPI_MSB;
    conf.max_hz = 500 * 1000;
    rt_spi_configure(this->spiDev, &conf);
}

void SpiHlw8112::cmd(int cmd, void* data, int len) {
    auto buf = shared_ptr<rt_uint8_t[]>(new rt_uint8_t[len]);
    reverseCopy(buf.get(), (rt_uint8_t*)data, len);
    rt_spi_send_then_send(spiDev, &cmd, sizeof(rt_uint8_t), buf.get(), len);
}

void SpiHlw8112::readReg(char addr, void* data, int len, rt_int32_t timeout) {
    auto buf = shared_ptr<rt_uint8_t[]>(new rt_uint8_t[len]);
    rt_spi_send_then_recv(spiDev, &addr, sizeof(addr), buf.get(), len);
    reverseCopy((rt_uint8_t*)data, buf.get(), len);
}

void SpiHlw8112::reverseCopy(rt_uint8_t* dst, const rt_uint8_t* src, int len) {
    for(auto i = 0; i < len; i++) {
        dst[i] = src[len - 1 - i];
    }
}
