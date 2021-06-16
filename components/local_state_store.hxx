#pragma once

#include "state_store.hxx"
#include <components/handshake.hxx>
#include <components/packet.hxx>
#include <components/rpc.hxx>
#include <memory>
#include <utilities/shared_thread.hxx>
#include <components/timer.hxx>

class LocalStateStore: public StateStore {
public:
    LocalStateStore(int idx, std::shared_ptr<Packet> packet, std::shared_ptr<Handshake> handshake, std::shared_ptr<SharedThread> thread, std::shared_ptr<Rpc> rpc);
private:
    void send(int idx, State::Value state);
private:
    std::shared_ptr<Packet> packet;
    std::shared_ptr<Handshake> handshake;
    std::shared_ptr<SharedThread> thread;
    std::shared_ptr<Rpc> rpc;
    Timer timer = {5 * 1000, "lss"};

};

#include <utilities/singleton.hxx>
namespace Preset {
template<int R>
class LocalStateStore: public Singleton<LocalStateStore<R>>, public ::LocalStateStore {
    friend class Singleton<LocalStateStore<R>>;
    LocalStateStore(): ::LocalStateStore(R, Packet::get(), Handshake::get(), SharedThread<Priority::Middle>::get(), Rpc::get()) {}
};
}
