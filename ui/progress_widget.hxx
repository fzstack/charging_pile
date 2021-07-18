/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-04-13     imgcr       the first version
 */
#pragma once
#include "widget.hxx"
#include "color_vel.hxx"

class ProgressWidget: public CSizeWidget<5, 2> {
    using p_t = CSizeWidget<5, 2>;
    using p_t::p_t;
public:
    void show(Colors::Argb color = kForeColor);
    void hide();
    void setProgress(int value);
protected:
    virtual void onDraw(std::shared_ptr<Graphics> graphics) override;
    virtual void onTick() override;
private:
    class PVel: public Vel<int> {
        using Vel<int>::Vel;
    public:
        virtual float ease(float x) override;
        virtual int intepl(const int& originVal, const int& targetVal, float pos) override;
    };

    ColorVel foreColor = {Colors::Argb::kTransparent, vel_step(500)}, backColor = {Colors::Argb::kTransparent, vel_step(2000)};
    PVel progress = {0, vel_step(1000)};
    static const Colors::Argb kForeColor, kBackColor;
};

#include <utilities/singleton.hxx>
#include "core.hxx"
#include "ws_screen.hxx"
namespace Preset {
class ProgressWidget: public Singleton<ProgressWidget>, public ::ProgressWidget {
    friend singleton_t;
    ProgressWidget(): ::ProgressWidget(0, 0, zIndex) {
        auto holder = std::shared_ptr<::ProgressWidget>(this, [](auto){});
        Core::get()->add(shared_from_this());
    }

    static const int zIndex;
};
}
