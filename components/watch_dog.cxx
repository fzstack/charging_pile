/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-04-10     imgcr       the first version
 */

#include "watch_dog.hxx"
#include <rtdevice.h>

WatchDog::WatchDog(uint32_t timeout): wdtDev(rt_device_find("wdt")) {
    rt_device_control(wdtDev, RT_DEVICE_CTRL_WDT_SET_TIMEOUT, &timeout);
    rt_device_control(wdtDev, RT_DEVICE_CTRL_WDT_START, RT_NULL);
    rt_thread_idle_sethook([](){
        auto self = Preset::WatchDog::get();
        self->feed();
    });

    timer.onRun += [this]{
        if(f.updateAndCheck()) {
            rt_kprintf("STOP WDT\n");
            feedCond = false;
        }
        feed();
    };

    timer.start();
}

void WatchDog::resetAfter(rt_uint8_t durationS) {
    f.setInitial(durationS);
    f.reset();
    f.retrigger();
}

void WatchDog::cancel() {
    f.reset();
}

void WatchDog::feed() {
    if(feedCond) {
        rt_device_control(wdtDev, RT_DEVICE_CTRL_WDT_KEEPALIVE, NULL);
    }
}
