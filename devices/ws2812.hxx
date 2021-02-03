/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-01-29     imgcr       the first version
 */
#ifndef DEVICES_WS2812_HXX_
#define DEVICES_WS2812_HXX_

#include <rtthread.h>
#include <memory>
#include <vector>
#include <rthw.h>
#include <ui/colors.hxx>

class Ws2812 {
public:
    Ws2812(rt_base_t dinPin, int num);
    std::shared_ptr<void> getSess();

    void flush();
    void reset();

    void test(Colors::Rgb c);

    Colors::Rgb& getPixel(int idx);
    std::size_t getCount();

    void writePixel(Colors::Rgb pixel);

private:

    void writeByte(rt_uint8_t b);

    void writeBit(int value);

    template<int N = 0>
    __attribute__((always_inline)) void delay() {
        __asm("MOV R0, R0");
        delay<N-1>();
    }

private:
    rt_base_t dinPin;
    std::vector<Colors::Rgb> frame;
    volatile uint32_t* bssr;
    const volatile uint32_t pin;

    static constexpr int
        kTMajor1000ns = 48, //47 48 50
        kTMinor1000ns = 20, //15  20
        kTReset1000ns = 100, //80
        kTMajorNs = 900,
        kTMinorNs = 290,
        kTResetNs = 80000,
        kMajorTimes = 40,
        kMinorTimes = 5,
        kResetTimes = kTResetNs * kTReset1000ns / 1000;
};

#include <utilities/singleton.hxx>
namespace Preset {
class Ws2812: public Singleton<Ws2812>, public ::Ws2812 {
    friend singleton_t;
    Ws2812(): ::Ws2812(kDinPin, kNum) {}
    static const rt_base_t kDinPin;
public:
    static const int kNum;
};
}

template<>
inline void Ws2812::delay<0>(){

}

#endif /* DEVICES_WS2812_HXX_ */
