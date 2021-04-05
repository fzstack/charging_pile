#pragma once

#include <utilities/nested.hxx>
#include <applications/thing.hxx>
#include <Mutex.h>
#include <Lock.h>
#include <utilities/inner_port.hxx>
#include <config/app.hxx>

class Thing;
class ChargerInfo;

namespace Things::Decos {
class Base: public Nested<Thing> {
    friend outer_t;
protected:
    using nested_t::Nested;
    virtual void init() {};
    virtual void query() {};
    virtual void config(DevConfig conf) {};
    virtual void onStateChanged(InnerPort port, State::Value state) {};
    virtual void onCurrentChanged(InnerPort port, int value) {};
    virtual void onVoltageChanged(InnerPort port, int value) {};
    ChargerInfo& getInfo(InnerPort port);
    rtthread::Lock getLock();
    rtthread::Mutex& getMutex();
};
}
