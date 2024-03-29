#pragma once
#include "app.hxx"
#include <config/bsp.hxx>
#include "ali_cloud.hxx"
#include "thing_preset.hxx"
#include "user.hxx"
#include <components/handshake.hxx>
#include <components/persistent_storage_preset.hxx>
#include <components/rebooter_preset.hxx>
#include <components/app_state.hxx>
#include <components/watch_dog.hxx>
#include <applications/ota.hxx>
#include "admin.hxx"
#include "loader_preset.hxx"
#include <components/debugger_preset.hxx>

class App;
class UpperApp: public App {
public:
    UpperApp();
    virtual void run() override;
private:
    std::shared_ptr<Handshake> handshake = Preset::Handshake::get(HandshakeType::Major); //~5k RAMA
    std::shared_ptr<ThingBase> thing = Preset::Thing::get(); //~6.4k RAM
    std::shared_ptr<Cloud> cloud = Preset::AliCloud::get(); //~11k RAM
    std::shared_ptr<User> user = Preset::User::get(); //~12k RAM
    std::shared_ptr<Admin> admin = Preset::Admin::get();
    // std::shared_ptr<Rebooter> rebooter = Preset::Rebooter::get();
    std::shared_ptr<AppState> state = Preset::AppState::get();
    std::shared_ptr<WatchDog> watchDog = Preset::WatchDog::get();
    std::shared_ptr<Ota> ota = Preset::Ota::get();
    std::shared_ptr<UpperLoader> loader = Preset::Loader::get();
    std::shared_ptr<Debugger> debugger = Preset::Debugger::get();
};
