/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-02-01     imgcr       the first version
 */
#pragma once

#include "widget.hxx"
#include <tuple>
#include <components/timer.hxx>

class LoadingWidget: public Widget {
public:
    LoadingWidget(int x, int y, int width, int height, int zIndex, std::shared_ptr<Timer> timer);
protected:
    virtual void onDraw(std::shared_ptr<Graphics> graphics) override;
private:
    float timing(float t);
    std::tuple<int, int> getPosFromIdx(int idx);
    static int particleNumFromSize(int width, int height);

    std::shared_ptr<Timer> timer;
    const int particleNum = particleNumFromSize(width, height);
    const float phaseStep = 1.f / (particleNum);
    float curVal = 0;
    static constexpr int kDuration = 1500;
    static const Colors::Argb kBaseColor;
};

#include <utilities/singleton.hxx>
#include "core.hxx"
#include "ws_screen.hxx"
namespace Preset {
class LoadingWidget: public Singleton<LoadingWidget>, public ::LoadingWidget {
    friend singleton_t;
    LoadingWidget(): ::LoadingWidget(0, 0, WsScreen::kWidth, WsScreen::kHeight, zIndex, std::make_shared<Timer>(kDuration, kTimer)) {
        auto holder = std::shared_ptr<::LoadingWidget>(this, [](auto){});
        Core::get()->add(shared_from_this());
    }

    static const int zIndex;
    static const int kDuration;
    static const char* kTimer;
};
}

