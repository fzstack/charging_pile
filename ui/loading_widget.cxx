/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-02-01     imgcr       the first version
 */

#include "loading_widget.hxx"
#include <cmath>
#include <utilities/math.hxx>

LoadingWidget::LoadingWidget(int x, int y, int width, int height, int zIndex, std::shared_ptr<Timer> timer): Widget(x, y, width, height, zIndex), timer(timer) {
    timer->onRun += [this]{
        invalid();
        curVal += (float)this->timer->getDuration() / kDuration;
        curVal = fmod(curVal, 1.f);
    };
    timer->start();
}

void LoadingWidget::onDraw(std::shared_ptr<Graphics> graphics) {
    for(auto i = 0; i < particleNum; i++) {
        auto [dx, dy] = getPosFromIdx(i);
        auto tarVal = fmod(curVal - phaseStep * i + 1, 1);
        auto curColor = kBaseColor;
        curColor.a *= timing(tarVal);
        graphics->setPixel(curColor, dx, dy);
    }
}

float LoadingWidget::timing(float t) {
    t = saturate(t, 0.f, 1.f);
    if(t <= 0.1) { //esse out quad
        t *= 10;
        return 1 - pow(1.f - t, 4);
    } else { //ease out expo
        t = (t - 0.1f) / 0.9f;
        return t > 0.8 ? 0 : pow(2, -10 * t);
    }
}

std::tuple<int, int> LoadingWidget::getPosFromIdx(int idx) {
    if(idx < width) {
        return {idx, 0};
    } else if(idx < width + height - 1) {
        return {width - 1, idx - width + 1};
    } else if(idx < 2 * width + height - 1) {
        return {width + 2 * height - idx, height - 1};
    } else {
        return {0, 2 * width + 2 * height - idx};
    }
}

int LoadingWidget::particleNumFromSize(int width, int height) {
    return height <= 1 ? width * height : width * 2 + (height - 2) * 2;
}

const Colors::Argb LoadingWidget::kBaseColor = {10, 255, 255, 255};

