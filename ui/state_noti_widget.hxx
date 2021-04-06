/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-04-06     imgcr       the first version
 */
#pragma once
#include "widget.hxx"
#include <array>
#include <config/bsp.hxx>
#include "color_vel.hxx"
#include <components/app_state.hxx>
#include <utilities/count_down.hxx>

class StateNotiWidget: public CSizeWidget<5, 2> {
    using p_t = CSizeWidget<5, 2>;
public:
    StateNotiWidget(int x, int y, int zIndex, std::shared_ptr<AppState> appState);
protected:
    virtual void onDraw(std::shared_ptr<Graphics> graphics) override;
    virtual void onTick() override;

    struct NotiPixel {
        ColorVel vel = {Colors::Argb::kBlack, vel_step(200, 50)};
        CountDown<> blink = {25};
        bool dir = false;
    };

private:
    std::array<NotiPixel, Config::Bsp::kPortNum> notis; //自然端口顺序
    std::shared_ptr<AppState> appState;

    static Colors::Argb kRed, kGreen, kBlue, kBlack;
};

#include <utilities/singleton.hxx>
#include "core.hxx"
#include "ws_screen.hxx"
namespace Preset {
class StateNotiWidget: public Singleton<StateNotiWidget>, public ::StateNotiWidget {
    friend singleton_t;
    StateNotiWidget(): ::StateNotiWidget(0, 0, zIndex, AppState::get()) {
        auto holder = std::shared_ptr<::StateNotiWidget>(this, [](auto){});
        Core::get()->add(shared_from_this());
    }

    static const int zIndex;
};
}
