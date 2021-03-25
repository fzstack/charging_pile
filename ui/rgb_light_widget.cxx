#include "rgb_light_widget.hxx"
#include <utilities/math.hxx>

RgbLightWidget::RgbLightWidget(int x, int y, int zIndex): widget_t(x, y, zIndex), RgbLight(), tween(Colors::Argb::kBlack, kDuration, kFreq) {
    tween.oCurrVal += [this](auto value) {
        invalid();
    };
    r.onChanged += [this](auto value) { update(); };
    g.onChanged += [this](auto value) { update(); };
    b.onChanged += [this](auto value) { update(); };
}

void RgbLightWidget::onDraw(std::shared_ptr<Graphics> graphics) {
    graphics->setPixel(*tween.oCurrVal, 0, 0);
}

void RgbLightWidget::update() {
    auto newTarget = Colors::Rgb::kBlack;
    if(*r && **r) {
        newTarget = lightColor(newTarget, kRed);
    }
    if(*g && **g) {
        newTarget = lightColor(newTarget, kGreen);
    }
    if(*b && **b) {
        newTarget = lightColor(newTarget, kBlue);
    }
    tween.setTargetVal(newTarget.toArgb(255));
}

Colors::Rgb RgbLightWidget::lightColor(Colors::Rgb a, Colors::Rgb b) {
    auto result = Colors::Rgb{};
    result.r = saturate<int>(a.r + b.r, 0, 255);
    result.g = saturate<int>(a.g + b.g, 0, 255);
    result.b = saturate<int>(a.b + b.b, 0, 255);
    return result;
}

Colors::Argb RgbLightWidget::ColorTween::update(Colors::Argb& initVal, Colors::Argb& targetVal, float pos) {
    auto result = Colors::Argb{};
    result.a = between(initVal.a, targetVal.a, pos);
    result.r = between(initVal.r, targetVal.r, pos);
    result.g = between(initVal.g, targetVal.g, pos);
    result.b = between(initVal.b, targetVal.b, pos);
    return result;
}

float RgbLightWidget::ColorTween::ease(float x) {
    return -x * x + 2 * x;
}

Colors::Rgb
    RgbLightWidget::kRed = {200, 35, 2},
    RgbLightWidget::kGreen = {60, 150, 5},
    RgbLightWidget::kBlue = {2, 140, 211};
