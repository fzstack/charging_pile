/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-01-03     imgcr       the first version
 */
#ifndef APPLICATIONS_EVENT_EMIT_THING_DECO_HXX_
#define APPLICATIONS_EVENT_EMIT_THING_DECO_HXX_

#include "thing_deco.hxx"
#include <utilities/observable.hxx>

class EventEmitThingDeco: public ThingDeco {
    friend outer_t;
    EventEmitThingDeco(outer_t* outer);
    virtual void init() override;

private:
    Observable<bool> inited = false;
};



#endif /* APPLICATIONS_EVENT_EMIT_THING_DECO_HXX_ */
