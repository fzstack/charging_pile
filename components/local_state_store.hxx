#pragma once

#include "state_store.hxx"
#include <components/handshake.hxx>
#include <components/packet.hxx>
#include <memory>

class LocalStateStore: public StateStore {
public:
    LocalStateStore(int idx, std::shared_ptr<Packet> packet, std::shared_ptr<Handshake> handshake);
private:
    std::shared_ptr<Packet> packet;
    std::shared_ptr<Handshake> handshake;
};

#include <utilities/singleton.hxx>
namespace Preset {
template<int R>
class LocalStateStore: public Singleton<LocalStateStore<R>>, public ::LocalStateStore {
    friend class Singleton<LocalStateStore<R>>;
    LocalStateStore(): ::LocalStateStore(R, Packet::get(), Handshake::get()) {}
};
}
