#include "loading_widget.hxx"
#include <cmath>
#include <utilities/math.hxx>
#include "core.hxx"

LoadingWidget::LoadingWidget(int x, int y, int zIndex): p_t(x, y, zIndex) {

}

void LoadingWidget::onDraw(std::shared_ptr<Graphics> graphics) {
    graphics->fillRect(kBackground, 0, 0, getWidth(), getHeight()); //80%透明度的黑色背景
    for(auto i = 0; i < particleNum; i++) {
        auto [dx, dy] = getPosFromIdx(i);
        auto tarVal = fmod(curVal - phaseStep * i + 1, 1);
        auto curColor = kBaseColor;
        curColor.a *= timing(tarVal);
        graphics->addPixel(curColor, dx, dy); //绘制loading条
    }
}

void LoadingWidget::onTick() {
    invalid();
    curVal += (float)Core::kDuration / kDuration;
    curVal = fmod(curVal, 1.f);
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
    if(idx < getWidth()) {
        return {idx, 0};
    } else if(idx < getWidth() + getHeight() - 1) {
        return {getWidth() - 1, idx - getWidth() + 1};
    } else if(idx < 2 * getWidth() + getHeight() - 1) {
        return {getWidth() + 2 * getHeight() - idx, getHeight() - 1};
    } else {
        return {0, 2 * getWidth() + 2 * getHeight() - idx};
    }
}

int LoadingWidget::particleNumFromSize(int width, int height) {
    return height <= 1 ? width * height : width * 2 + (height - 2) * 2;
}

const Colors::Argb
    LoadingWidget::kBaseColor = {50, 255, 255, 255},
    LoadingWidget::kBackground = {240, 0, 0, 0};

