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
    info.charger->start();
    info.timerId = timerId;
    info.leftSeconds = minutes * 60;
}

void Thing::stop(int port, int timerId) {
    auto& info = infos[port];
    info.charger->stop();
    info.timerId = timerId;
}

void Thing::config(int currentLimit, int uploadThr, int fuzedThr) {
    for(auto deco: decos) {
        deco->config(currentLimit, uploadThr, fuzedThr);
    }
}

#include <array>
#include <utilities/mp.hxx>
#include "countdown_thing_deco.hxx"
#include "event_emit_thing_deco.hxx"
#include "current_limit_thing_deco.hxx"
#include "backup_thing_deco.hxx"
#include "data_set_thing_deco.hxx"
#include "consumption_measure_thing_deco.hxx"

namespace Preset {
Thing::Thing(): ::Thing(getChargers(), User::get(), LastCharger::get()) {
    addDeco<EventEmitThingDeco>(); //事件上报功能
    addDeco<CountdownThingDeco>(); //倒计时功能
    addDeco<CurrentLimitThingDeco>(); //限流功能
    addDeco<BackupThingDeco>(); //状态备份功能
    addDeco<DataSetThingDeco>(); //状态数据上报功能
    addDeco<ConsumptionMeasureThingDeco>(); //功耗测量功能
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
