/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-12-28     imgcr       the first version
 */

#include "cloud.hxx"
#include <config/app.hxx>

Cloud::Cloud(std::shared_ptr<AliIotDevice> device, std::shared_ptr<Air724> air): device(device), air(air) {

}

void Cloud::init() {
    air->init();
    device->login(Config::App::cloudDeviceName, Config::App::cloudProductKey, Config::App::cloudDeviceSecret);
}
