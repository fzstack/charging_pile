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

StateNotiWidget::StateNotiWidget(int x, int y, int zIndex, std::shared_ptr<AppState> appState): p_t(x, y, zIndex), appState(appState) {
    appState->portStateChanged += [this](NatPort port, auto state) {
        auto& noti = notis[port.get() - NatPort::min().get()];
        switch(state) {
        case State::LoadNotInsert:
            noti.blink.reset();
            noti.vel.setTarget(kGreen);
            break;
        case State::LoadInserted:
            noti.blink.retrigger();
            noti.dir = false;
            noti.vel.setTarget(kRed);
            break;
        case State::Charging:
            noti.blink.retrigger();
            noti.dir = false;
            noti.vel.setTarget(kGreen);
            break;
        case State::LoadWaitRemove:
            noti.blink.reset();
            noti.vel.setTarget(kBlue);
            break;
        default:
            break;
        }
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
            switch(s) {
            case State::LoadInserted:
            case State::Charging: {
                auto color = noti.dir ? (s == State::Charging ? kGreen: kRed): Colors::Argb::kBlack;
                noti.blink.retrigger();
                noti.vel.setTarget(color);
                noti.dir = !noti.dir;
                break;
            }
            default:
                break;
            }
        }
        notis[i].vel.update();
    }
    invalid();
}

Colors::Argb
    StateNotiWidget::kRed = {255, 200, 35, 2},
    StateNotiWidget::kGreen = {255, 60, 150, 5},
    StateNotiWidget::kBlue = {255, 2, 140, 211};

