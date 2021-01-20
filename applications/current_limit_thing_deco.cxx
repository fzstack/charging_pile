/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-01-19     imgcr       the first version
 */

#include "current_limit_thing_deco.hxx"
#include <components/persistent_storage.hxx>
#include <config/app.hxx>

using namespace std;

CurrentLimitThingDeco::CurrentLimitThingDeco(outer_t* outer): ThingDeco(outer) {

    //每个端口一个定时器
    for(auto& timer: timers) {
        timer = make_shared<Timer>(kDuration, kTimer, RT_TIMER_FLAG_ONE_SHOT);
    }

    inited.onChanged += [this](auto value) {
        if(!value) return;
        for(auto i = 0u; i < Config::Bsp::kPortNum; i++) {
            auto charger = getInfo(i).charger;
            auto timer = timers[i];

            charger->multimeterChannel->current += [charger, timer, i](auto value) {
                auto storage = Preset::PersistentStorage::get();
                auto config = storage->make<Config::Thing>();
                if(!value) return;

                rt_kprintf("port%d current: %dmA\n", i, *value);
                if(*value > config->maxCurrentMiA) {
                    if(!timer->isRunning() && charger->stateStore->oState.value() == State::Charging) timer->start();
                } else {
                    if(timer->isRunning()) timer->stop();
                }
            };

            timer->onRun += [charger, i, this]() {
                if(charger->stateStore->oState.value() == State::Charging) {
                    charger->stop();
                    this->outer->onCurrentLimit(i);
                }
            };
        }
    };
}

void CurrentLimitThingDeco::init() {
    inited = true;
}

