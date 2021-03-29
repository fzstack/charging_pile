#pragma once

#include <utilities/nested.hxx>
#include <applications/thing.hxx>
#include <Lock.h>
#include <utilities/inner_port.hxx>

class Thing;
class ChargerInfo;

namespace Things::Decos {
class Base: public Nested<Thing> {
    friend outer_t;
protected:
    using nested_t::Nested;
    virtual void init() = 0;
    virtual void query() {};
    virtual void config(int currentLimit, int uploadThr, int fuzedThr, int noloadCurrThr) {};
    ChargerInfo& getInfo(InnerPort port);
    rtthread::Lock getLock();
};
}
