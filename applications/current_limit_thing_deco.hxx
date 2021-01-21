/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-01-19     imgcr       the first version
 */
#ifndef APPLICATIONS_CURRENT_LIMIT_THING_DECO_HXX_
#define APPLICATIONS_CURRENT_LIMIT_THING_DECO_HXX_

#include "thing_deco.hxx"
#include <utilities/observable.hxx>
#include <array>
#include <components/timer.hxx>
#include <config/bsp.hxx>
#include <Mutex.h>

class CurrentLimitThingDeco: public ThingDeco {
    friend outer_t;
    CurrentLimitThingDeco(outer_t* outer);
    virtual void init() override;

    Observable<bool> inited = false;
    std::array<std::shared_ptr<Timer>, Config::Bsp::kPortNum> timers;

    rtthread::Mutex mutex;

private:
    static constexpr int kDuration = 2000;
    static const char* kTimer;
    static const char* kMutex;
};



#endif /* APPLICATIONS_CURRENT_LIMIT_THING_DECO_HXX_ */
