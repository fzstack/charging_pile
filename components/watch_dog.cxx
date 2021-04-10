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
    auto ret = rt_device_control(wdtDev, RT_DEVICE_CTRL_WDT_SET_TIMEOUT, &timeout);
    rt_device_control(wdtDev, RT_DEVICE_CTRL_WDT_START, RT_NULL);
    rt_thread_idle_sethook([](){
        auto self = Preset::WatchDog::get();
        rt_device_control(self->wdtDev, RT_DEVICE_CTRL_WDT_KEEPALIVE, NULL);
    });
}

