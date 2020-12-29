/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-12-29     imgcr       the first version
 */

#include "timer.hxx"

using namespace std;

Timer::Timer(rt_tick_t time, const char* name): timer(std::shared_ptr<rt_timer>(rt_timer_create(name, [](auto p){
    auto timer = (Timer*)p;
    timer->run();
}, this, time, RT_TIMER_FLAG_PERIODIC | RT_TIMER_FLAG_SOFT_TIMER), rt_timer_delete)) {

}

void Timer::start() {
    rt_timer_start(timer.get());
}

