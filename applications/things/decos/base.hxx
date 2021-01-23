/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-01-03     imgcr       the first version
 */
#pragma once

#include <utilities/nested.hxx>
#include <applications/thing.hxx>
#include <Lock.h>

class Thing;
class ChargerInfo;

namespace Things::Decos {
class Base: public Nested<Thing> {
    friend outer_t;
protected:
    using nested_t::Nested;
    virtual void init() = 0;
    virtual void config(int currentLimit, int uploadThr, int fuzedThr, int noloadCurrThr) {};
    ChargerInfo& getInfo(int port);
    std::shared_ptr<User> getUser();
    std::shared_ptr<LastCharger> getLast();
    rtthread::Lock getLock();
};
}
