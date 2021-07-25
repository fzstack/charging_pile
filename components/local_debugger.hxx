#pragma once
#include "debugger.hxx"
#include <components/rebooter_preset.hxx>
#include <components/packet.hxx>

//TODO: upper
class LocalDebugger: public Debugger {
public:
    LocalDebugger(std::shared_ptr<Rebooter> rebooter, std::shared_ptr<Packet> packet);
    virtual void debug(DebugType type) override;

private:
    std::shared_ptr<Rebooter> rebooter;
    std::shared_ptr<Packet> packet;
};

#include <utilities/singleton.hxx>
namespace Preset {
class LocalDebugger: public Singleton<LocalDebugger>, public ::LocalDebugger {
    friend class Singleton<LocalDebugger>;
    LocalDebugger(): ::LocalDebugger(Rebooter::get(), Packet::get()) {}
};
}
