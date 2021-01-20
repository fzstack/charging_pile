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
}

