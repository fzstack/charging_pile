#include "local_thing.hxx"
#include <config/co.hxx>
#include <stdexcept>

using namespace std;
using namespace Rpcs;
using namespace Packets;

LocalThing::LocalThing(
    std::array<std::shared_ptr<Charger>, Config::Bsp::kPortNum> chargers,
    std::shared_ptr<Packet> packet,
    std::shared_ptr<Rpc> rpc
): Thing(chargers), packet(packet), rpc(rpc) {
    rpc->def<Services::Control>([this](auto p) {
        control(p->port, p->timerId, p->minutes);
    });
    rpc->def<Services::Stop>([this](auto p) {
        stop(p->port, p->timerId);
    });
    rpc->def<Services::Config>([this](auto p){
        config(p->currentLimit, p->uploadThr, p->fuzedThr, p->noloadCurrThr);
    });
    rpc->def<Services::Query>([this](auto p) {
        query();
    });

    onPortAccess += [this](auto port) {
        this->packet->emit<Events::PortAccess>({port});
    };
    onPortUnplug += [this](auto port) {
        this->packet->emit<Events::PortUnplug>({port});
    };
    onCurrentLimit += [this](auto port) {
        this->packet->emit<Events::CurrentLimit>({port});
    };

    onCurrentData += [this](auto data){
        this->packet->emit<Events::CurrentData>({data});
    };
}

#ifdef LOWER_END
#include <things/decos/counter.hxx>
#include <things/decos/event_emitter.hxx>
#include <things/decos/current_limiter.hxx>
#include <things/decos/backuper.hxx>
#include <things/decos/data_setter.hxx>
#include <things/decos/consumption_measurer.hxx>
#include <things/decos/fuse_detecter.hxx>
#include <things/decos/noload_detecter.hxx>
using namespace Things::Decos;
namespace Preset {
LocalThing::LocalThing(): ::LocalThing(Chargers::get(), Packet::get(), Rpc::get()) {
    addDeco<EventEmitter>();
    addDeco<Counter>();
    addDeco<CurrentLimiter>();
    //addDeco<Backuper>();
    addDeco<DataSetter>();
    addDeco<ConsumptionMeasurer>();
    //addDeco<FuseDetecter>();
    addDeco<NoloadDetecter>();
    init();
}
}
#endif

