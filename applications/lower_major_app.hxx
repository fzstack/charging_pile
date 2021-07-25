#pragma once
#include "lower_app.hxx"
#include "thing_preset.hxx"
#include "components/handshake.hxx"
#include <components/rebooter_preset.hxx>
#include <components/ota_lower.hxx>
#include <components/watch_dog.hxx>
#include <components/lower/comm_guard.hxx>
#include <components/debugger_preset.hxx>

class App;
class LowerMajorApp: public LowerApp {
public:
    LowerMajorApp();
    virtual void run() override;
    virtual std::shared_ptr<Handshake> getHandshake() override;
private:
    std::shared_ptr<Handshake> handshake = Preset::Handshake::get(HandshakeType::Major);
    std::shared_ptr<ThingBase> thing = Preset::Thing::get();
    std::shared_ptr<OtaLower> otaLower = Preset::OtaLower::get();
    std::shared_ptr<WatchDog> watchDog = Preset::WatchDog::get();
    std::shared_ptr<CommGuard> commGuard = Preset::CommGuard::get();
    std::shared_ptr<Debugger> debugger = Preset::Debugger::get();
};
