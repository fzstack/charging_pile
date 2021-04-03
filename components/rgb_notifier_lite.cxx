/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-04-01     imgcr       the first version
 */

#include "rgb_notifier_lite.hxx"
#include <config/bsp.hxx>

RgbNotifierLite::RgbNotifierLite(std::shared_ptr<AppState> state, std::shared_ptr<Ws2812> ws2812) {
    state->portStateChanged += [this](auto port, auto state) {
        fUpdate.retrigger();
    };

    timer.onRun += [=] {
        i++;
        i %= 10;
        if(i % 5 == 0) {
            fUpdate.retrigger();
        }

        if(fUpdate.updateAndCheck()) {
            for(rt_uint8_t j = 0; j < Config::Bsp::kPortNum; j++) {
                auto s = state->getPortState(InnerPort{j});
                auto& p = ws2812->getPixel(j);
                auto b = (i + j) % 10 < 5;
                switch(s) {
                case State::LoadNotInsert:
                    p = kGreen;
                    break;
                case State::LoadInserted:
                    p = b ? kBlack : kRed;
                    break;
                case State::Charging:
                    p = b ? kBlack : kGreen;
                    break;
                case State::LoadWaitRemove:
                    p = kBlue;
                    break;
                case State::Error:
                    p = b ? kGreen : kRed;
                    break;
                }
            }
            ws2812->flush();
        }
    };

    timer.start();
}

Colors::Rgb
    RgbNotifierLite::kRed = {200, 35, 2},
    RgbNotifierLite::kGreen = {60, 150, 5},
    RgbNotifierLite::kBlue = {2, 140, 211},
    RgbNotifierLite::kBlack = {0, 0, 0};
