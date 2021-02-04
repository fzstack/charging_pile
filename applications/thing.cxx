/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-01-01     imgcr       the first version
 */

#include "thing.hxx"
#include <Lock.h>

using namespace std;
using namespace string_literals;
using namespace rtthread;

Thing::Thing(array<shared_ptr<Charger>, Config::Bsp::kPortNum> chargers, shared_ptr<User> user, shared_ptr<LastCharger> last):
    infos(), user(user), last(last) {

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

void Thing::control(int port, int timerId, int minutes) {
    auto guard = Lock(mutex);
    auto& info = infos[port]; //NOTE: std::array会自动进行边界检查
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

void Thing::stop(int port, int timerId) {
    auto& info = infos[port];
    info.charger->stop();
    info.timerId = timerId;
}

void Thing::config(int currentLimit, int uploadThr, int fuzedThr, int noloadCurrThr) {
    for(auto deco: decos) {
        deco->config(currentLimit, uploadThr, fuzedThr, noloadCurrThr);
    }
}

#include <array>
#include <utilities/mp.hxx>
#include <things/decos/counter.hxx>
#include <things/decos/event_emitter.hxx>
#include <things/decos/current_limiter.hxx>
#include <things/decos/backuper.hxx>
#include <things/decos/data_setter.hxx>
#include <things/decos/consumption_measurer.hxx>
#include <things/decos/fuse_detecter.hxx>
#include <things/decos/noload_detecter.hxx>

#ifdef LOWER_END
using namespace Things::Decos;
namespace Preset {
Thing::Thing(): ::Thing(getChargers(), User::get(), LastCharger::get()) {
    addDeco<EventEmitter>();
    addDeco<Counter>();
    addDeco<CurrentLimiter>();
    addDeco<Backuper>();
    addDeco<DataSetter>();
    addDeco<ConsumptionMeasurer>();
    addDeco<FuseDetecter>();
    addDeco<NoloadDetecter>();
    init();
}

std::array<std::shared_ptr<::Charger>, Config::Bsp::kPortNum> Thing::getChargers() {
    auto result = std::array<std::shared_ptr<::Charger>, Config::Bsp::kPortNum>();
    for(auto i = 0u; i < result.size(); i++) {
        magic_switch<Config::Bsp::kPortNum>{}([&](auto v){
            result[i] = Charger<decltype(v)::value>::get();
        }, i);
    }
    return result;
}
}
#endif
