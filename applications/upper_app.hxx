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

class App;
class UpperApp: public App {
public:
    UpperApp();
    virtual void run() override;
private:
    std::array<std::shared_ptr<RgbStateNotifier>, Config::Bsp::kPortNum> rgbNotifiers;
};


