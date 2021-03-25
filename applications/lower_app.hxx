#pragma once

#include "app.hxx"
#include "thing_preset.hxx"

class App;
class LowerApp: public App {
public:
    virtual void run() override;
private:
    std::shared_ptr<ThingBase> thing = Preset::Thing::get();
};


