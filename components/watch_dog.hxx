/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-04-10     imgcr       the first version
 */
#pragma once
#include <rtthread.h>

class WatchDog {
public:
    WatchDog(uint32_t timeout);
private:
    rt_device_t wdtDev;
};

#include <utilities/singleton.hxx>
namespace Preset {
class WatchDog: public Singleton<WatchDog>, public ::WatchDog {
    friend singleton_t;
    WatchDog(): ::WatchDog(kTimeoutS) {}
    static constexpr int kTimeoutS = 5;
};
}
