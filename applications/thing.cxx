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

#include "auto_poweroff_thing_deco.hxx"
#include "event_emit_thing_deco.hxx"
namespace Preset {
Thing::Thing(): ::Thing({Charger<0>::get(), Charger<1>::get()}, User::get(), LastCharger::get()) {
    addDeco<EventEmitThingDeco>();
    addDeco<AutoPoweroffThingDeco>();
    init();
}
}
