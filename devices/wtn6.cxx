/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-08-26     imgcr       the first version
 */

#include "wtn6.hxx"
#include <rtthread.h>
#include <rtdevice.h>
#include <rthw.h>
#include <utilities/shared_thread.hxx>

#define LOG_TAG "dev.wtn6"
#define LOG_LVL LOG_LVL_DBG
#include <ulog.h>

Wtn6::Wtn6(rt_base_t dataPin, rt_base_t busyPin): dataPin(dataPin), busyPin(busyPin) {
    rt_pin_mode(dataPin, PIN_MODE_OUTPUT);
    rt_pin_mode(busyPin, PIN_MODE_INPUT);
    rt_pin_write(dataPin, PIN_HIGH);
}

bool Wtn6::isBusy() {
    return rt_pin_read(busyPin) == PIN_LOW;
}

void Wtn6::write(rt_uint8_t data) {
    auto high = Preset::SharedThread<Priority::High>::get();
    high->exec([this, data]{
        rt_pin_write(dataPin, PIN_LOW);
        rt_thread_mdelay(5);
        rt_pin_write(dataPin, PIN_HIGH);
        for(auto i = 0; i < 8; i++) {
            writeBit((data & (1 << i)) != 0);
        }
        rt_pin_write(dataPin, PIN_HIGH);
    });
}

void Wtn6::writeBit(bool bit) {
    auto delayHigh = 200, delayLow = 600;
    if(bit) {
        delayHigh = 600;
        delayLow = 200;
    }

    rt_pin_write(dataPin, PIN_HIGH);
    rt_hw_us_delay(delayHigh);
    rt_pin_write(dataPin, PIN_LOW);
    rt_hw_us_delay(delayLow);
}
