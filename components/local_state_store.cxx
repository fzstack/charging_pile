#include "local_state_store.hxx"
#include <components/packet.hxx>
#include <config/bsp.hxx>
#include <config/co.hxx>
#include <utilities/mp.hxx>
#include <utilities/inner_port.hxx>

LocalStateStore::LocalStateStore(int idx, std::shared_ptr<Packet> packet, std::shared_ptr<Handshake> handshake): packet(packet), handshake(handshake) {
    oState += [this, idx](auto value) {
        if(!value) return;
        this->packet->emit<Packets::State>({InnerPort{(rt_uint8_t)idx}, *value});
    };

    handshake->done += [this, idx] {
        auto state = oState.value();
        if(!state) return;
        this->packet->emit<Packets::State>({InnerPort{(rt_uint8_t)idx}, *state});
    };
}
