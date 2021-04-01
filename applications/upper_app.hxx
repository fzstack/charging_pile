#pragma once
#include "app.hxx"
#include <config/bsp.hxx>
#include "ali_cloud.hxx"
#include "thing_preset.hxx"
#include "user.hxx"
#include <components/handshake.hxx>
#include <components/persistent_storage_preset.hxx>

class App;
class UpperApp: public App {
public:
    UpperApp();
    virtual void run() override;
private:
    std::shared_ptr<ThingBase> thing = Preset::Thing::get();
    std::shared_ptr<Cloud> cloud = Preset::AliCloud::get();
    std::shared_ptr<User> user = Preset::User::get();
    std::shared_ptr<Handshake> handshake = Preset::Handshake::get();
    std::shared_ptr<LocalFalPersistentStorage> storage = Preset::PersistentStorage::get();
};
