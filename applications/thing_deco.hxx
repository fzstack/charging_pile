/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-01-03     imgcr       the first version
 */
#ifndef APPLICATIONS_THING_DECO_HXX_
#define APPLICATIONS_THING_DECO_HXX_

#include <utilities/nested.hxx>
#include "thing.hxx"

class Thing;
class ChargerInfo;

class ThingDeco: public Nested<Thing> {
    friend outer_t;
protected:
    using nested_t::Nested;
    virtual void init() = 0;
    ChargerInfo& getInfo(int port);
    std::shared_ptr<User> getUser();
    std::shared_ptr<LastCharger> getLast();
};



#endif /* APPLICATIONS_THING_DECO_HXX_ */
