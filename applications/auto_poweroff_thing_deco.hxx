/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-01-03     imgcr       the first version
 */
#ifndef APPLICATIONS_AUTO_POWEROFF_THING_DECO_HXX_
#define APPLICATIONS_AUTO_POWEROFF_THING_DECO_HXX_

#include "thing_deco.hxx"
#include <utilities/observable.hxx>

class AutoPoweroffThingDeco: public ThingDeco {
    friend outer_t;
    AutoPoweroffThingDeco(outer_t* outer);
    virtual void init() override;

    Observable<bool> inited = false;

    Timer timer = {1000, kTimer};
    static const char* kTimer;
};



#endif /* APPLICATIONS_AUTO_POWEROFF_THING_DECO_HXX_ */
