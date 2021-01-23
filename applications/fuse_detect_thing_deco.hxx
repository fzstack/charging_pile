/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-01-23     imgcr       the first version
 */
#ifndef APPLICATIONS_FUSE_DETECT_THING_DECO_HXX_
#define APPLICATIONS_FUSE_DETECT_THING_DECO_HXX_

#include "thing_deco.hxx"
#include <utilities/observable.hxx>
#include <config/bsp.hxx>
#include <list>
#include <array>
#include <components/timer.hxx>

class FuseDetectThingDeco: public ThingDeco {
    friend outer_t;
    FuseDetectThingDeco(outer_t* outer);
    virtual void init() override;
    virtual void config(int currentLimit, int uploadThr, int fuzedThr, int noloadCurrThr) override;

    struct Params {
        int fuzedS2Thr = 10000000;
    };

    struct ChargerSpec {
        std::list<float> angleHist = {};
    };

    static const char* kTimer;
    static constexpr int kDuration = 1000;
    static constexpr int kSatSize = 10;
    Timer timer = {kDuration, kTimer};
    std::array<ChargerSpec, Config::Bsp::kPortNum> specs;
    Observable<bool> inited = false;
};



#endif /* APPLICATIONS_FUSE_DETECT_THING_DECO_HXX_ */
