/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-01-20     imgcr       the first version
 */

#include "app.hxx"
#include "thing.hxx"
#include "ali_cloud.hxx"

using namespace std;

void runApp() {
    auto cloud = Preset::AliCloud::get();
    auto thing = Preset::Thing::get();

    cloud->onControl += [=](int port, int timerId, int minutes) {
        try {
            thing->control(port, timerId, minutes);
            return Cloud::ServiceResult::Succeed;
        } catch(const exception& e) {
            return Cloud::ServiceResult::Failed;
        }
    };

    cloud->onStop += [=](int port, int timerId) {
        try {
            thing->stop(port, timerId);
            return Cloud::ServiceResult::Succeed;
        } catch(const exception& e) {
            return Cloud::ServiceResult::Failed;
        }
    };

    cloud->onQuery += [=] {
        thing->onCurrentData();
    };

    cloud->onConfig += [=](int currentLimit, int uploadThr, int fuzedThr) {
        thing->config(currentLimit, uploadThr, fuzedThr);
    };

    thing->onIcNumber += cloud->post([=](int port, string icNumber) {
        cloud->emitIcNumber(port, icNumber);
    });

    thing->onPortAccess += cloud->post([=](int port) {
        cloud->emitPortAccess(port);
    });

    thing->onPortUnplug += cloud->post([=](int port) {
       cloud->emitPortUnplug(port);
    });

    thing->onCurrentLimit += cloud->post([=](int port) {
        cloud->emitCurrentLimit(port);
    });

    thing->onCurrentData += cloud->post([=] {
        thing->getCurrentData([&](auto result){
            auto data = get_if<std::array<CurrentData, Config::Bsp::kPortNum>>(&result);
            if(!data) return;
            cloud->setCurrentData(*data);
        });
    });

    cloud->init();

//    {
//        "port": 0,
//        "timer_id": 1,
//        "minutes": 5
//    }


}

