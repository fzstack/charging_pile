#include "remote_state_store.hxx"
#include <components/packet.hxx>
#include <config/bsp.hxx>
#include <config/co.hxx>
#include <utilities/mp.hxx>

RemoteStateStore::RemoteStateStore(int idx) {

}

void RemoteStateStore::on(State::Value value) {
    this->state = value;
}

void RemoteStateStore::staticCtor() {
    auto packet = Preset::Packet::get();
    packet->on<Packets::State>([](auto p) {
        magic_switch<Config::Bsp::kPortNum>{}([&](auto v) {
            Preset::RemoteStateStore<decltype(v)::value>::get()->on(p->value);
        }, p->port.get());
    });
}
