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

//std::array<CurrentData, Config::Bsp::kPortNum> Thing::getCurrentData() {
//    auto arr = array<CurrentData, Config::Bsp::kPortNum>{};
//    for(rt_uint8_t i = 0u; i < Config::Bsp::kPortNum; i++) {
//        auto& info = infos[i];
//        auto charger = info.charger;
//        arr[i] = CurrentData {
//            port: InnerPort{i},
//            timerId: info.timerId,
//            leftMinutes: info.leftSeconds / 60,
//            state: **charger->stateStore->oState,
//            current: (float)**charger->multimeterChannel->current / 1000.f,
//            voltage: (float)**charger->multimeterChannel->voltage,
//            consumption: info.consumption,
//            fuse: CurrentData::Fuse::Normal,
//        };
//    }
//    return arr;
//}
