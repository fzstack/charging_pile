/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-08-25     imgcr       the first version
 */

#include "multimeter.hxx"
#include <rtthread.h>

#define LOG_TAG "com.multimeter"
#define LOG_LVL LOG_LVL_DBG
#include <ulog.h>

using namespace std;

Multimeter::Multimeter(std::shared_ptr<Hlw8112> device): device(device) {
    inited.onChanged += [this](auto value) {
        if(value) {
            this->device->init();
            timer = shared_ptr<rt_timer>(rt_timer_create(kTimer, [](auto p) {
                auto self = (Multimeter*)p;

                try {
                    auto valA = *self->device->makeSess<rms_i_a>();
                    auto valB = *self->device->makeSess<rms_i_b>();
                    auto valU = *self->device->makeSess<rms_u>();

                    self->curChA = int(self->calcCurrent(valA.data, self->curCChA));
                    self->curChB = int(self->calcCurrent(valB.data, self->curCChB));
                    self->vol = int(self->calcVoltage(valU.data, self->volC));
                } catch(const exception& e) {
                    rt_kprintf("\033[39m{%s} %s\n\033[0m", typeid(e).name(), e.what());
                }

            }, this, kTimerDurMs, RT_TIMER_FLAG_PERIODIC | RT_TIMER_FLAG_SOFT_TIMER), [](auto p) {
                rt_timer_stop(p);
                rt_timer_delete(p);
            });

            curCChA = *this->device->makeSess<rms_i_a_c>();
            curCChB = *this->device->makeSess<rms_i_b_c>();
            volC = *this->device->makeSess<rms_u_c>();

            rt_timer_start(timer.get());
        } else {
            timer = nullptr;
        }
    };
}

void Multimeter::init() {
    inited = true;
}

std::shared_ptr<Multimeter::Channel> Multimeter::getChannel(ChPort port) {
    switch(port) {
     case ChPort::A:
         return std::make_shared<Channel>(curChA, vol);
         break;
     case ChPort::B:
         return std::make_shared<Channel>(curChB, vol);
         break;
    }
    return nullptr;
}

float Multimeter::calcCurrent(float val, float valc) {
    return 1.0 * val * valc / (1 << 23) / 1.7;
}

float Multimeter::calcVoltage(float val, float valc) {
    return 1.0 * val * valc / (1.0 * (1 << 22)) / 100;
}

