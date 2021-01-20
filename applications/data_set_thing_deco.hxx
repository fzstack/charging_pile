/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-01-21     imgcr       the first version
 */
#ifndef APPLICATIONS_DATA_SET_THING_DECO_HXX_
#define APPLICATIONS_DATA_SET_THING_DECO_HXX_

#include "thing_deco.hxx"
#include <utilities/observable.hxx>

class DataSetThingDeco: public ThingDeco {
    friend outer_t;
    DataSetThingDeco(outer_t* outer);
    virtual void init() override;

    Observable<bool> inited = false;
};



#endif /* APPLICATIONS_DATA_SET_THING_DECO_HXX_ */
