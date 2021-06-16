#include "remote_state_store.hxx"
#include <components/packet.hxx>
#include <config/bsp.hxx>
#include <config/co.hxx>
#include <utilities/mp.hxx>
#include <components/rpc.hxx>

RemoteStateStore::RemoteStateStore(int idx) {

}

void RemoteStateStore::on(State::Value value) {
    this->state = value;
}

void RemoteStateStore::staticCtor() {
    auto rpc = Preset::Rpc::get();
    rt_kprintf("remote state store static stor\n");
    rpc->def<Rpcs::State>([](auto p) {
        magic_switch<Config::Bsp::kPortNum>{}([&](auto v) {
            Preset::RemoteStateStore<decltype(v)::value>::get()->on(p->value);
        }, p->port.get());
    });
}
