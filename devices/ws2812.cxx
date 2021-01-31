/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-01-29     imgcr       the first version
 */

#include "ws2812.hxx"
#include <rtdevice.h>
#include <utilities/shared_thread.hxx>
#include <stm32f1xx.h>

struct pin_index
{
    int index;
    GPIO_TypeDef *gpio;
    uint32_t pin;
};
extern const struct pin_index pins[];

Ws2812::Ws2812(rt_base_t dinPin, int num): dinPin(dinPin), frame(num) {
    rt_pin_mode(dinPin, PIN_MODE_OUTPUT);
    rt_pin_write(dinPin, PIN_HIGH);
    //bssr = &pins[dinPin].gpio->BSRR;
    bssr = &GPIOC->BSRR;
}

std::shared_ptr<void> Ws2812::getSess() {
    return std::shared_ptr<void>(nullptr, [this](auto) {
        flush();
    });
}

void Ws2812::flush() {
    auto high = Preset::SharedThread<Priority::High>::get();
    high->exec([this](){
        reset();
        for(auto i = 0; i < frame.size(); i++) {
            writePixel(frame[i]);
        }
    });
}

void Ws2812::reset() {
    //rt_pin_write(dinPin, PIN_LOW);
    *bssr = 1 << (4 + 16);
    delay<kResetTimes>();
    //rt_pin_write(dinPin, PIN_HIGH);
    *bssr = 1 << (4);
    *bssr = 1 << (4 + 16);
}

void Ws2812::test(Colors::Rgb c) {
    auto high = Preset::SharedThread<Priority::High>::get();
    high->exec([this, c]{
        reset();
        for(auto i = 0; i < 10; i++) {
            writePixel(c);
        }
    });
}

Colors::Rgb& Ws2812::getPixel(int idx) {
    return frame[idx];
}

std::size_t Ws2812::getCount() {
    return frame.size();
}

void Ws2812::writePixel(Colors::Rgb pixel) {
    writeByte(pixel.r);
    writeByte(pixel.g);
    writeByte(pixel.b);
}

void Ws2812::writeByte(rt_uint8_t b) {
    for(auto i = 7; i >= 0; i--) {
        writeBit(b & (1 << i));
    }
}

void Ws2812::writeBit(int value) {
    //*bssr = 1 << (1); //for test
    auto ctx = rt_hw_interrupt_disable();
    if(value) {
        //delay<42>();
        *bssr = 1 << (4);
        __asm("nop");__asm("nop");__asm("nop");__asm("nop");__asm("nop");__asm("nop");__asm("nop");__asm("nop");__asm("nop");__asm("nop");
        __asm("nop");__asm("nop");__asm("nop");__asm("nop");__asm("nop");__asm("nop");__asm("nop");__asm("nop");__asm("nop");__asm("nop");
        __asm("nop");__asm("nop");__asm("nop");__asm("nop");__asm("nop");__asm("nop");__asm("nop");__asm("nop");__asm("nop");__asm("nop");
        __asm("nop");

        //delay<7>(); //11
        *bssr = 1 << (4 + 16);
        __asm("nop");__asm("nop");__asm("nop");__asm("nop");__asm("nop");__asm("nop");

    } else {
        //delay<7>();
        //delay<4>();
        //delay<3>();
        *bssr = 1 << (4);
        __asm("nop");__asm("nop");__asm("nop");__asm("nop");__asm("nop");__asm("nop");__asm("nop");__asm("nop");__asm("nop");__asm("nop");

        //delay<43>();
        *bssr = 1 << (4 + 16);
        __asm("nop");__asm("nop");__asm("nop");__asm("nop");__asm("nop");__asm("nop");__asm("nop");__asm("nop");__asm("nop");__asm("nop");
        __asm("nop");__asm("nop");__asm("nop");__asm("nop");__asm("nop");__asm("nop");__asm("nop");__asm("nop");__asm("nop");__asm("nop");
        __asm("nop");__asm("nop");__asm("nop");

    }
    //*bssr = 1 << (1 + 16); //for test
    rt_hw_interrupt_enable(ctx);
}


