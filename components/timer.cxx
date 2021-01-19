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

Timer::Timer(rt_tick_t time, const char* name, rt_uint8_t flags): timer(std::shared_ptr<rt_timer>(rt_timer_create(name, [](auto p){
    auto timer = (Timer*)p;
    timer->run();
}, this, time, flags | RT_TIMER_FLAG_SOFT_TIMER), rt_timer_delete)), flags(flags) {

}

void Timer::start() {
    if(running) return;
    rt_timer_start(timer.get());
    running = true;
}

void Timer::stop() {
    if(!running) return;
    rt_timer_stop(timer.get());
    running = false;
}

bool Timer::isRunning() {
    return running;
}

void Timer::run() {
    if(!(flags & RT_TIMER_FLAG_PERIODIC)) {
        running = false;
    }
    onRun();

}
