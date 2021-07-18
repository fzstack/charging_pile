#include "local_state_store.hxx"
#include <components/packet.hxx>
#include <config/bsp.hxx>
#include <config/co.hxx>
#include <utilities/mp.hxx>
#include <utilities/inner_port.hxx>
#include <utilities/retry.hxx>

LocalStateStore::LocalStateStore(int idx, std::shared_ptr<Packet> packet, std::shared_ptr<Handshake> handshake, std::shared_ptr<SharedThread> thread, std::shared_ptr<Rpc> rpc): packet(packet), handshake(handshake), thread(thread), rpc(rpc) {
    oState += [this, idx](auto value) {
        if(!value) return;
        send(idx, *value);
    };

    handshake->oConnected += [this, idx](auto value) {
        if(!value) return;
        auto state = oState.value();
        if(!state) return;
        send(idx, *state);
    };

    // timer.onRun += [this, idx](){
    //     send(idx, oState.value().value_or(State::LoadNotInsert));
    // };
}

void LocalStateStore::send(int idx, State::Value state) {
    packet->emit<Packets::State>({InnerPort{(rt_uint8_t)idx}, state});
}
