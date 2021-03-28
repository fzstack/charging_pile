#include "local_state_store.hxx"
#include <components/packet.hxx>
#include <config/bsp.hxx>
#include <config/co.hxx>
#include <utilities/mp.hxx>
#include <utilities/inner_port.hxx>

LocalStateStore::LocalStateStore(int idx) {
    oState += [idx](auto value) {
        if(!value) return;
        auto packet = Preset::Packet::get();
        packet->emit<Packets::State>({InnerPort{(rt_uint8_t)idx}, *value});
    };
}
