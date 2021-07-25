#pragma once
#include "debugger.hxx"
#include <components/rebooter_preset.hxx>
#include <components/packet.hxx>

class RemoteDebugger: public Debugger {
public:
    RemoteDebugger(std::shared_ptr<Rebooter> rebooter, std::shared_ptr<Packet> packet);
private:
    std::shared_ptr<Rebooter> rebooter;
    std::shared_ptr<Packet> packet;
};

#include <utilities/singleton.hxx>
namespace Preset {
class RemoteDebugger: public Singleton<RemoteDebugger>, public ::RemoteDebugger {
    friend class Singleton<RemoteDebugger>;
    RemoteDebugger(): ::RemoteDebugger(Rebooter::get(), Packet::get()) {}
};
}
