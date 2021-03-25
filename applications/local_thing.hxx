#pragma once

#include "thing.hxx"
#include <memory>
#include <components/packet.hxx>
#include <components/rpc.hxx>

class LocalThing: public Thing {
public:
    LocalThing(std::array<std::shared_ptr<Charger>, Config::Bsp::kPortNum> chargers, std::shared_ptr<Packet> packet, std::shared_ptr<Rpc> rpc);
private:
    std::shared_ptr<Packet> packet;
    std::shared_ptr<Rpc> rpc;
};

#include <utilities/singleton.hxx>
namespace Preset {
class LocalThing: public Singleton<LocalThing>, public ::LocalThing {
    friend class Singleton<LocalThing>;
    LocalThing();
};
}
