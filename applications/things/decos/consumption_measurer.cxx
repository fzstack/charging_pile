/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-01-21     imgcr       the first version
 */

#include "consumption_measurer.hxx"
#include <Lock.h>

using namespace std;
using namespace rtthread;
using namespace Things::Decos;

ConsumptionMeasurer::ConsumptionMeasurer(outer_t* outer): Base(outer), mutex(kMutex) {
    inited.onChanged += [this](auto value) {
        if(!value) return;
        for(auto i = 0u; i < Config::Bsp::kPortNum; i++) {
            auto& info = getInfo(i);
            auto& spec = specs[i];
            auto charger = info.charger;
            spec.timer = make_shared<Timer>(kDuration, kTimer);
            spec.timer->start();
            charger->stateStore->oState += [this, &info, &spec](auto state) {
                if(!state) return;
                auto guard = Lock(mutex);
                switch(*state) {
                case State::Charging: //开始充电后耗电量清零，开启定时器
                    info.consumption = 0;
                    spec.tick = rt_tick_get();
                    spec.prevCurr = info.charger->multimeterChannel->current.value().value_or(0);
                    spec.prevVol = info.charger->multimeterChannel->voltage.value().value_or(0);
                    break;
                case State::LoadWaitRemove: //充电完成，关闭定时器  TODO: consumption也写入需保存的状态中
                    //spec.timer->stop();
                    break;
                case State::LoadNotInsert: //负载拔出后耗电量清零
                    info.consumption = 0;
                    break;
                default:
                    break;
                }
            };

            charger->multimeterChannel->current += [this, &info, &spec](auto value) {
                if(!value) return;
                update(info, spec);
            };

            charger->multimeterChannel->voltage += [this, &info, &spec](auto value) {
                if(!value) return;
                update(info, spec);
            };

            spec.timer->onRun += [this, &info, &spec] {
                update(info, spec);
            };
        }
    };
}

void ConsumptionMeasurer::update(ChargerInfo& info, ChargerSpec& spec) {
    auto guard = Lock(mutex);
    if(*info.charger->stateStore->oState != State::Charging) return;
    info.consumption += spec.prevCurr / 1000.f * spec.prevVol * (rt_tick_get() - spec.tick) / 1000.f;
    spec.tick = rt_tick_get();
    spec.prevCurr = info.charger->multimeterChannel->current.value().value_or(0);
    spec.prevVol = info.charger->multimeterChannel->voltage.value().value_or(0);
}

void ConsumptionMeasurer::init() {
    inited = true;
}
