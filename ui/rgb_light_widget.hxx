#pragma once

#include "widget.hxx"
#include <ui/colors.hxx>
#include <devices/rgb_light.hxx>
#include <utilities/tween.hxx>

class RgbLightWidget: public CSizeWidget<1, 1>, public RgbLight {
    using widget_t = CSizeWidget<1, 1>;
public:
    RgbLightWidget(int x, int y, int zIndex);

protected:
    virtual void onDraw(std::shared_ptr<Graphics> graphics) override;

private:
    void update();
    Colors::Rgb lightColor(Colors::Rgb a, Colors::Rgb b);

private:
    class ColorTween: public Tween<Colors::Argb> {
        using Tween<Colors::Argb>::Tween;
    protected:
        virtual Colors::Argb update(Colors::Argb& initVal, Colors::Argb& targetVal, float pos) override;
        virtual float ease(float x) override;
    private:
        template<class T>
        T between(T& a, T& b, float pos) {
            return a + (b - a) * pos;
        }
    };

private:
    ColorTween tween;
    static Colors::Rgb kRed, kGreen, kBlue;
    static constexpr int kSize = 1;
    static constexpr int kDuration = 350;
    static constexpr int kFreq = 50;
};

#include <utilities/singleton.hxx>
#include "core.hxx"
#include "ws_screen.hxx"
namespace Preset {
template<int R>
class RgbLightWidget: public Singleton<RgbLightWidget<R>>, public ::RgbLightWidget {
    friend Singleton<RgbLightWidget<R>>;
    RgbLightWidget(): ::RgbLightWidget(R % WsScreen::kWidth, R / WsScreen::kWidth, zIndex) {
        auto holder = std::shared_ptr<::RgbLightWidget>(this, [](auto){});
        Core::get()->add(shared_from_this());
    }

    static const int zIndex;
};
}

