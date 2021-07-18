/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-04-13     imgcr       the first version
 */

#include "progress_widget.hxx"
#include <utilities/math.hxx>
#include <cmath>

void ProgressWidget::show(Colors::Argb color) {
    foreColor.setTarget(color);
    backColor.setTarget(kBackColor);
}

void ProgressWidget::hide() {
    foreColor.setTarget(Colors::Argb::kTransparent);
    backColor.setTarget(Colors::Argb::kTransparent);
}

void ProgressWidget::setProgress(int value) {
    progress.setTarget(value);
}

void ProgressWidget::onDraw(std::shared_ptr<Graphics> graphics) {
    graphics->fillRect(backColor.get(), 0, 0, getWidth(), getHeight());
    //绘制实色区
    auto len = (float)getWidth() * progress.get() / 100;
    graphics->addRect(foreColor.get(), 0, 0, len, getHeight());
    auto rem = fmodf(len, 1.f);
    auto lineColor = foreColor.get();
    lineColor.a *= rem;
    graphics->addVLine(lineColor, len, 0, getHeight() - 1);
}

void ProgressWidget::onTick() {
    foreColor.update();
    backColor.update();
    progress.update();
    invalid();
}

float ProgressWidget::PVel::ease(float x) {
    return 1 - (1 - x) * (1 - x);
}

int ProgressWidget::PVel::intepl(const int& originVal, const int& targetVal, float pos) {
    return between(originVal, targetVal, pos);
}

const Colors::Argb
    ProgressWidget::kForeColor = {90, 100, 0, 200},
    ProgressWidget::kBackColor = {200, 0, 0, 0};
