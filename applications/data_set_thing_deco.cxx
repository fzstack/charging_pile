/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-01-21     imgcr       the first version
 */

#include "data_set_thing_deco.hxx"
#include <config/bsp.hxx>

using namespace std;

DataSetThingDeco::DataSetThingDeco(outer_t* outer): ThingDeco(outer) {
    inited.onChanged += [this](auto value) {
        if(!value) return;
        this->outer->getCurrentData += [this] {
            auto arr = array<CurrentData, Config::Bsp::kPortNum>{};
            for(auto i = 0u; i < Config::Bsp::kPortNum; i++) {
                auto& info = getInfo(i);
                auto charger = info.charger;
                arr[i] = CurrentData {
                    port: int(i),
                    timerId: info.timerId,
                    leftMinutes: info.leftSeconds / 60,
                    state: **charger->stateStore->oState,
                    current: (float)**charger->multimeterChannel->current / 1000.f,
                    voltage: (float)**charger->multimeterChannel->voltage,
                    consumption: 0.f,
                    fuse: CurrentData::Fuse::Normal,
                };
            }
            return arr;
        };

        for(auto i = 0u; i < Config::Bsp::kPortNum; i++) {
            auto charger = getInfo(i).charger;
            charger->stateStore->oState += [this](auto state) {
                if(!state) return;
                switch(*state) {
                case State::Charging:
                case State::LoadWaitRemove:
                    this->outer->onCurrentData();
                    break;
                default:
                    break;
                };
            };
        }
    };
}

void DataSetThingDeco::init() {
    inited = true;
}
