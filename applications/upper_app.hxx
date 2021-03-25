#pragma once
#include "app.hxx"
#include <config/bsp.hxx>
#include "ali_cloud.hxx"
#include "thing_preset.hxx"
#include "user.hxx"

class App;
class UpperApp: public App {
public:
    UpperApp();
    virtual void run() override;
private:
    std::shared_ptr<ThingBase> thing = Preset::Thing::get();
    std::shared_ptr<Cloud> cloud = Preset::AliCloud::get();
    std::shared_ptr<User> user = Preset::User::get();
};
