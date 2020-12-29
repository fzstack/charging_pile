/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-12-29     imgcr       the first version
 */

#include <rtthread.h>
#include <memory>

class Timer {
public:
    Timer(rt_tick_t time, const char* name);
    virtual void start();
protected:
    virtual void run() = 0;
private:
    std::shared_ptr<rt_timer> timer;
};

