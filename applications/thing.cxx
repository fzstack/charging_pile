#include "thing.hxx"
#include <Lock.h>

using namespace std;
using namespace string_literals;
using namespace rtthread;

Thing::Thing(array<shared_ptr<Charger>, Config::Bsp::kPortNum> chargers):
    infos() {

    transform(chargers.begin(), chargers.end(), infos.begin(), [](auto charger) {
        return ChargerInfo {
            charger: charger,
        };
    });

    inited.onChanged += [this](auto value) {
        if(!value) return;
        for(auto i = 0u; i < this->infos.size(); i++) {
            auto& info =  this->infos[i];
            auto charger = info.charger;
            charger->init();
        }
        for(auto& deco: decos) {
            deco->init();
        }
    };
}

void Thing::init() {
    inited = true;
}

void Thing::query() {
    //TODO
}

void Thing::control(InnerPort port, int timerId, int minutes) {
    auto guard = Lock(mutex);
    auto& info = infos[port.get()]; //NOTE: std::array会自动进行边界检查
    auto oldTimerId = info.timerId;
    info.leftSeconds = minutes * 60;
    info.timerId = timerId;
    try {
        info.charger->start();
    } catch(const exception& e) {
        info.leftSeconds = 0;
        info.timerId = oldTimerId;
    }
}

void Thing::stop(InnerPort port, int timerId) {
    auto& info = infos[port.get()];
    info.charger->stop();
    info.timerId = timerId;
}

void Thing::config(int currentLimit, int uploadThr, int fuzedThr, int noloadCurrThr) {
    for(auto deco: decos) {
        deco->config(currentLimit, uploadThr, fuzedThr, noloadCurrThr);
    }
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
ThingPre::ThingPre(): ::Thing(Chargers::get()) {
    addDeco<EventEmitter>();
    //addDeco<Counter>();
    addDeco<CurrentLimiter>();
    addDeco<Backuper>();
//    addDeco<DataSetter>();
//    addDeco<ConsumptionMeasurer>();
//    //addDeco<FuseDetecter>();
//    addDeco<NoloadDetecter>();
    init();
}
}
#endif
