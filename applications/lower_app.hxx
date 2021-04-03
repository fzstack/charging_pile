#pragma once

#include "app.hxx"
#include "thing_preset.hxx"
#include "components/backup_man_preset.hxx"
#include "components/handshake.hxx"

class App;
class LowerApp: public App {
public:
    LowerApp();

    virtual void run() override;
private:
    std::shared_ptr<ThingBase> thing = Preset::Thing::get();
    std::shared_ptr<Handshake> handshake = Preset::Handshake::get();
};


