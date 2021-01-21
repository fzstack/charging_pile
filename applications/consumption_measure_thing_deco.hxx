/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-01-21     imgcr       the first version
 */
#ifndef APPLICATIONS_CONSUMPTION_MEASURE_THING_DECO_HXX_
#define APPLICATIONS_CONSUMPTION_MEASURE_THING_DECO_HXX_

#include "thing_deco.hxx"
#include <utilities/observable.hxx>
#include <components/timer.hxx>
#include <rtthread.h>
#include <array>
#include <config/bsp.hxx>
#include <Mutex.h>

class ConsumptionMeasureThingDeco: public ThingDeco {
    friend outer_t;
    ConsumptionMeasureThingDeco(outer_t* outer);
    virtual void init() override;

    struct ChargerSpec {
        std::shared_ptr<Timer> timer;
        rt_tick_t tick;
        int prevCurr = 0;
        int prevVol = 0;
    };

    void update(ChargerInfo& info, ChargerSpec& spec);

    Observable<bool> inited = false;
    std::array<ChargerSpec, Config::Bsp::kPortNum> specs;
    rtthread::Mutex mutex;

    static constexpr int kDuration = 1000;
    static const char* kTimer;
    static const char* kMutex;
};



#endif /* APPLICATIONS_CONSUMPTION_MEASURE_THING_DECO_HXX_ */
