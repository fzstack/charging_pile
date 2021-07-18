#pragma once
#include "lower_app.hxx"
#include <components/handshake.hxx>
#include <components/ota_lower.hxx>
#include <components/watch_dog.hxx>
#include <components/lower/comm_guard.hxx>

class LowerOtaApp: public LowerApp {
public:
    virtual void run() override;
    virtual std::shared_ptr<Handshake> getHandshake() override;

    std::shared_ptr<Handshake> handshake = Preset::Handshake::get(HandshakeType::Ota);
    std::shared_ptr<OtaLower> otaLower = Preset::OtaLower::get();
    std::shared_ptr<WatchDog> watchDog = Preset::WatchDog::get();
    std::shared_ptr<CommGuard> commGuard = Preset::CommGuard::get();

};