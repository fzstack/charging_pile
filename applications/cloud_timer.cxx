/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-12-29     imgcr       the first version
 */

#include "cloud_timer.hxx"
#include <devices/air724.hxx>
#include <components/air_components.hxx>

CloudTimer::CloudTimer(outer_t* outer, rt_tick_t time, const char* name): nested_t(outer), Timer(time, name) {

}

void CloudTimer::start() {
    Timer::start();
}

void CloudTimer::run() {
    outer->setSignalInterval();
}
