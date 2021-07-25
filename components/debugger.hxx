#pragma once
#include <components/rebooter_preset.hxx>
#include <config/app.hxx>

class Debugger {
public:
    virtual void debug(DebugType type) {};
};