/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-02-03     imgcr       the first version
 */

#include "upper_app.hxx"
#include <rtthread.h>
#include <utilities/mp.hxx>

using namespace std;

UpperApp::UpperApp() {
    for(int i = 0; i < Config::Bsp::kPortNum; i++) {
        magic_switch<Config::Bsp::kPortNum>{}([&](auto v) {
            rgbNotifiers[i] = Preset::RgbStateNotifier<decltype(v)::value>::get();
        }, i);
    }
}

void UpperApp::run() {
    cloud->onControl += [=](auto port, auto timerId, auto minutes) {
        try {
            thing->control(port, timerId, minutes);
            return Cloud::ServiceResult::Succeed;
        } catch(const exception& e) {
            return Cloud::ServiceResult::Failed;
        }
    };
    cloud->onStop += [=](auto port, auto timerId) {
        try {
            thing->stop(port, timerId);
            return Cloud::ServiceResult::Succeed;
        } catch(const exception& e) {
            return Cloud::ServiceResult::Failed;
        }
    };

    cloud->onQuery += [=]{
        auto curData = thing->getCurrentData();
        cloud->setCurrentData(std::move(curData));
    };

    thing->onIcNumber += [=](int port, string icNumber) {
        cloud->emitIcNumber(port, icNumber);
    };
    thing->onPortAccess += [=](int port) {
        cloud->emitPortAccess(port);
    };
    thing->onPortUnplug += [=](int port) {
        if(rt_tick_get() < 1000) return;
        cloud->emitPortUnplug(port);
    };
    thing->onCurrentLimit += [=](int port) {
        cloud->emitCurrentLimit(port);
    };
    thing->onCurrentData += [=]{
        runOn(Preset::AliIotDevice::get()->thread->post([=]{
            auto curData = thing->getCurrentData();
            cloud->setCurrentData(std::move(curData));
        }));
    };
    cloud->init();
}

