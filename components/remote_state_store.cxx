#include "remote_state_store.hxx"
#include <components/packet.hxx>
#include <config/bsp.hxx>
#include <config/co.hxx>
#include <utilities/mp.hxx>

RemoteStateStore::RemoteStateStore(int idx) {
    auto packet = Preset::Packet::get();
    magic_switch<Config::Bsp::kPortNum>{}([&](auto v) {
        packet->on<Packets::State<decltype(v)::value>>([this](auto p){
            this->state = p->value;
        });
    }, idx);
}

