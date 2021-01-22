/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-01-23     imgcr       the first version
 */
#ifndef APPLICATIONS_NOLOAD_DETECT_THING_DECO_HXX_
#define APPLICATIONS_NOLOAD_DETECT_THING_DECO_HXX_

#include "thing_deco.hxx"
#include <utilities/observable.hxx>
#include <components/timer.hxx>
#include <array>
#include <config/bsp.hxx>

class NoloadDetectThingDeco: public ThingDeco {
    friend outer_t;
    NoloadDetectThingDeco(outer_t* outer);
    virtual void init() override;

    struct Params {
        int noloadCurrThr = 10; //空载电流阈值
    };

    struct ChargerSpec {
        Timer timer = {kDuration, kTimer, RT_TIMER_FLAG_ONE_SHOT};
    };

    Observable<bool> inited = false;
    std::array<ChargerSpec, Config::Bsp::kPortNum> specs;
    static const char* kTimer;
    static constexpr int kDuration = 4000; //空载判断区间
};



#endif /* APPLICATIONS_NOLOAD_DETECT_THING_DECO_HXX_ */
