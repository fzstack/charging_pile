/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-02-03     imgcr       the first version
 */

#include "upper_app.hxx"
#include <rtthread.h>
#include <utilities/mp.hxx>

UpperApp::UpperApp() {
    for(int i = 0; i < Config::Bsp::kPortNum; i++) {
        magic_switch<Config::Bsp::kPortNum>{}([&](auto v) {
            rgbNotifiers[i] = Preset::RgbStateNotifier<decltype(v)::value>::get();
        }, i);
    }
}

void UpperApp::run() {
    rt_kprintf("upper app runned\n");
}

