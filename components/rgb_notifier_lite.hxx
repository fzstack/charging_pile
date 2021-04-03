/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-04-01     imgcr       the first version
 */
#pragma once
#include <components/app_state.hxx>
#include <devices/ws2812.hxx>
#include <components/timer.hxx>
#include <utilities/count_down.hxx>

class RgbNotifierLite {
public:
    RgbNotifierLite(std::shared_ptr<AppState> state, std::shared_ptr<Ws2812> ws2812);

private:
    CountDown<> fUpdate = {};
    Timer timer = {100, "rgblt"};
    int i = 0;
    static Colors::Rgb kRed, kGreen, kBlue, kBlack;
};

#include <utilities/singleton.hxx>
namespace Preset {
class RgbNotifierLite: public Singleton<RgbNotifierLite>, public ::RgbNotifierLite {
    friend singleton_t;
    RgbNotifierLite(): ::RgbNotifierLite(AppState::get(), Ws2812::get()) { }
};
}
