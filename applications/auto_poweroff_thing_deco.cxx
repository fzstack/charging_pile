/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-01-03     imgcr       the first version
 */

#include "auto_poweroff_thing_deco.hxx"
#include <config/bsp.hxx>

AutoPoweroffThingDeco::AutoPoweroffThingDeco(outer_t* outer): ThingDeco(outer) {
    timer.onRun += [this](){
        for(auto i = 0u; i < Config::Bsp::kPortNum; i++) {
            auto& info = getInfo(i);
            auto& charger = info.charger;
            auto& leftSeconds = info.leftSeconds;
            if(*charger->stateStore->oState == State::Charging) {
                if(--leftSeconds <= 0) {
                    charger->stop();
                }
                rt_kprintf("[%d] remains %ds\n", i, leftSeconds);
            }
        }
    };

    inited.onChanged += [this](auto value) {
        if(!value) return;
        for(auto i = 0u; i < Config::Bsp::kPortNum; i++) {
            auto& info = getInfo(i);
            info.charger->stateStore->oState += [this, &info](auto value) {
                if(value != State::LoadWaitRemove) return;
                info.leftSeconds = 0; //充电完成则将剩余时间清零
            };
        }
        timer.start();
    };
}

void AutoPoweroffThingDeco::init() {
    inited = true;
}

