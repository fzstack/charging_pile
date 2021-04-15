#pragma once

#include "app.hxx"
#include "thing_preset.hxx"
#include "components/backup_man_preset.hxx"
#include "components/handshake.hxx"
#include <components/rebooter_preset.hxx>
#include <components/ota_lower.hxx>

class App;
class LowerApp: public App {
public:
    LowerApp();

    virtual void run() override;
private:
    std::shared_ptr<ThingBase> thing = Preset::Thing::get();
    std::shared_ptr<Handshake> handshake = Preset::Handshake::get();
    std::shared_ptr<Rebooter> rebooter = Preset::Rebooter::get();
    std::shared_ptr<OtaLower> otaLower = Preset::OtaLower::get();
};


