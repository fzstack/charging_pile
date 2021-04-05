#pragma once
#include <utilities/signals.hxx>
#include <string>
#include <array>
#include <config/app.hxx>
#include <config/bsp.hxx>
#include <utilities/inner_port.hxx>

class ThingBase {
public:
    virtual void init() = 0;
    virtual void query() = 0;
    virtual void control(InnerPort port, int timerId, int minutes) = 0;
    virtual void stop(InnerPort port, int timerId) = 0;
    virtual void config(DevConfig conf) = 0;
    virtual DevConfig readConfig() = 0;

    Signals<void(InnerPort port)> onPortAccess;
    Signals<void(InnerPort port)> onPortUnplug;
    Signals<void(InnerPort port)> onCurrentLimit;
    Signals<void(CurrentData data)> onCurrentData;
};


