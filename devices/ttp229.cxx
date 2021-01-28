/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-01-28     imgcr       the first version
 */

#include "ttp229.hxx"
#include <rthw.h>
#include <rtdevice.h>

Ttp229::Ttp229(std::shared_ptr<Thread> thread, rt_base_t sclPin, rt_base_t sdoPin): thread(thread), sclPin(sclPin), sdoPin(sdoPin), event() {
    rt_pin_mode(sclPin, PIN_MODE_OUTPUT);
    rt_pin_write(sclPin, PIN_LOW);
    rt_pin_mode(sdoPin, PIN_MODE_INPUT_PULLDOWN);
    rt_pin_attach_irq(sdoPin, PIN_IRQ_MODE_RISING, [](auto p) {
        //有下降沿时, 发送事件
        auto self = (Ttp229*)p;
        self->event.set();
    }, this);
    rt_pin_irq_enable(sdoPin, PIN_IRQ_ENABLE);

    thread->onRun += [this](){
        while(true) {
            event.wait();
            rt_pin_irq_enable(this->sdoPin, PIN_IRQ_DISABLE);
            rt_hw_us_delay(kTwUis);
            rt_uint16_t val = 0;
            for(auto i = 0; i < 16; i++) {
                rt_pin_write(this->sclPin, PIN_HIGH);
                rt_hw_us_delay(kTHalfSclUis);
                auto curVal = rt_pin_read(this->sdoPin);
                val |= curVal != 0 ? (1 << i) : 0;
                rt_pin_write(this->sclPin, PIN_LOW);
                rt_hw_us_delay(kTHalfSclUis);
            }
            rt_pin_irq_enable(this->sdoPin, PIN_IRQ_ENABLE);
            onData(val);
        }
    };
}
