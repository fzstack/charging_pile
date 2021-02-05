/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-02-03     imgcr       the first version
 */
#pragma once
#include "app.hxx"
#include <config/bsp.hxx>
#include <components/rgb_state_notifier.hxx>
#include <array>
#include <devices/wtn6_preset.hxx>
#include "ali_cloud.hxx"
#include "thing_preset.hxx"

class App;
class UpperApp: public App {
public:
    UpperApp();
    virtual void run() override;
private:
    std::array<std::shared_ptr<RgbStateNotifier>, Config::Bsp::kPortNum> rgbNotifiers;
    std::shared_ptr<Wtn6Base> wtn6 = Preset::Wtn6::get();
    std::shared_ptr<ThingBase> thing = Preset::Thing::get();
    std::shared_ptr<Cloud> cloud = Preset::AliCloud::get();
};


