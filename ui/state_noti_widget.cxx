/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-04-06     imgcr       the first version
 */

#include "state_noti_widget.hxx"
#include <utilities/math.hxx>

StateNotiWidget::StateNotiWidget(int x, int y, int zIndex, std::shared_ptr<AppState> appState): p_t(x, y, zIndex), appState(appState) {
    appState->portStateChanged += [this](NatPort port, auto state) {
        if(state != State::Charging && state != State::LoadNotInsert) return;
        rt_kprintf("[%d] port %d -> %d\n", rt_tick_get(), port.get(), state);
        auto& noti = notis[port.get() - NatPort::min().get()];
        noti.blink.forceTrigger();
        noti.dir = false;
        noti.vel.setTarget(state == State::Charging ? getChargingColor(port): kWhite);
    };
}

void StateNotiWidget::onDraw(std::shared_ptr<Graphics> graphics) {
    for(rt_uint8_t i = 0; i < Config::Bsp::kPortNum; i++) {
        graphics->setPixel(notis[i].vel.get(), i % getWidth(), i / getWidth());
    }
}

void StateNotiWidget::onTick() {
    for(rt_uint8_t i = 0; i < Config::Bsp::kPortNum; i++) {
        auto& noti = notis[i];
        if(noti.blink.updateAndCheck()) {
            auto port = NatPort{rt_uint8_t(i + NatPort::min().get())};
            auto s = appState->getPortState(port);
            auto color = noti.dir ? (s == State::Charging ? getChargingColor(port): kWhite): Colors::Argb::kBlack;
            noti.blink.retrigger();
            noti.vel.setTarget(color);
            noti.dir = !noti.dir;
        }
        noti.vel.update();
    }
    invalid();
}

Colors::Argb StateNotiWidget::getChargingColor(InnerPort port) {
    auto current = appState->getCurrent(port);
    auto pos = saturate(current / 2000.f, 0.f, 1.f);
    return Colors::Argb::intepl(kGreen, kRed, pos);
}

Colors::Argb
    StateNotiWidget::kRed = {255, 200, 35, 2},
    StateNotiWidget::kGreen = {255, 60, 150, 5},
    StateNotiWidget::kBlue = {255, 2, 140, 211},
    StateNotiWidget::kWhite = {255, 117, 117, 117};

