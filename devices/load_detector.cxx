/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-08-24     imgcr       the first version
 */
#include "load_detector.hxx"
#include <rtdevice.h>

//正常情况下每20ms一个脉冲
using namespace std;

//TODO: 需要能添加更多规则
LoadDetector::LoadDetector(rt_base_t pin): pin(pin), oState(state) {
    rt_pin_mode(pin, PIN_MODE_INPUT_PULLDOWN);
    rt_pin_attach_irq(pin, PIN_IRQ_MODE_RISING, [](auto p) {
        auto self = (LoadDetector*)p;
        self->ticks[self->rear] = rt_tick_get();
        self->rear++;
        self->rear %= self->kArraySize;

        if(!self->fulled && self->rear == 0) {
            self->fulled = true;
        }
    }, this);
    rt_pin_irq_enable(pin, PIN_IRQ_ENABLE);

    timer.onRun += [this]{
        auto size = kArraySize;
        if(!fulled)
            size = rear;
        auto now = rt_tick_get();
        auto count = int{};
        for(auto i = 0; i < size; i++) {
            if(now - ticks[i] < kDetectWndMs)
                count++;
        }
        state = count >= kArraySize * kValidRatioPc / 100;
    };

    timer.start();
}

Timer LoadDetector::timer = {kDetectWndMs, kTimer};
