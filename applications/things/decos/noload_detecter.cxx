/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-01-23     imgcr       the first version
 */

#include <config/bsp.hxx>
#include <components/persistent_storage.hxx>
#include "noload_detecter.hxx"

using namespace Things::Decos;

NoloadDetecter::NoloadDetecter(outer_t* outer): Base(outer) {
    inited.onChanged += [this](auto value) {
        if(!value) return;
        for(auto i = 0u; i < Config::Bsp::kPortNum; i++) {
            auto& info = getInfo(i);
            auto& spec = specs[i];
            auto charger = info.charger;
            auto& timer = spec.timer;

            charger->stateStore->oState += [&timer](auto state) {
                if(!state) return;
                switch(*state) {
                case State::LoadWaitRemove:
                    timer.stop();
                    break;
                default: break;
                }
            };

            charger->multimeterChannel->current += [charger, &timer](auto value) {
                if(!value) return;
                if(*charger->stateStore->oState != State::Charging) return;
                auto params = Preset::PersistentStorage::get()->make<Params>();
                if(*value < params->noloadCurrThr) {
                    if(!timer.isRunning()) {
                        timer.start();
                    }
                } else {
                    if(timer.isRunning()) {
                        timer.stop();
                    }
                }
            };

            timer.onRun += [charger]{
                if(*charger->stateStore->oState == State::Charging) {
                    rt_kprintf("stop chargering due to no-load\n");
                    charger->stop();
                }
            };
        }
    };
}

void NoloadDetecter::init() {
    inited = true;
}

void NoloadDetecter::config(int currentLimit, int uploadThr, int fuzedThr, int noloadCurrThr) {
    auto params = Preset::PersistentStorage::get()->make<Params>();
    params->noloadCurrThr = noloadCurrThr;
}
