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
#include <utilities/json.hxx>

Cloud::Cloud(std::shared_ptr<AliIotDevice> device, std::shared_ptr<Air724> air, std::shared_ptr<CloudTimer> timer): device(device), air(air), timer(timer) {

}

void Cloud::init() {
    air->init();
    device->login(Config::App::cloudDeviceName, Config::App::cloudProductKey, Config::App::cloudDeviceSecret);

    auto ess = air->make<AirEssential>();
    setIccid(ess->getIccid());
    timer->start();
}

void Cloud::setCurrentData(std::array<CurrentData, Config::Bsp::kPortNum> data) {
    auto value = Json::array({});
    for(const auto& item: data) {
        value.push_back({
            {"port", item.port},
            {"timer_id", item.timerId},
            {"left_minutes", item.leftMinutes},
            {"state", item.state},
            {"current", item.current},
            {"voltage", item.voltage},
            {"consumption", item.consumption},
            {"fuse", item.fuse},
        });
    }
    device->set("current_data", value);
}

void Cloud::setIccid(std::string_view iccid) {
    device->set("iccid", iccid);
}

namespace Preset {
Cloud::Cloud(): ::Cloud(AliIotDevice::get(), Air724::get(), CloudTimer::get(this)) {}
}
