#pragma once

#include "widget.hxx"
#include <tuple>
#include "color_vel.hxx"

class LoadingWidget: public CSizeWidget<5, 2> {
    using p_t = CSizeWidget<5, 2>;
public:
    LoadingWidget(int x, int y, int zIndex);

    enum class Type {
        Info,
        Warn,
        Error,
    };

    void setType(Type type);
    void show();
    void hide();
protected:
    virtual void onDraw(std::shared_ptr<Graphics> graphics) override;
    virtual void onTick() override;
private:
    float timing(float t);
    std::tuple<int, int> getPosFromIdx(int idx);
    static int particleNumFromSize(int width, int height);
    bool isShow = true;

    const int particleNum = particleNumFromSize(getWidth(), getHeight());
    const float phaseStep = 1.f / (particleNum);
    float curVal = 0;
    Colors::Argb baseColor = kInfoColor;
    ColorVel foreColor = {baseColor, vel_step(500)}, backColor = {kBackground, vel_step(2000)};

    static constexpr int kDuration = 1700;
    static const Colors::Argb kInfoColor, kWarnColor, kErrorColor, kBackground;
};

#include <utilities/singleton.hxx>
#include "core.hxx"
#include "ws_screen.hxx"
namespace Preset {
class LoadingWidget: public Singleton<LoadingWidget>, public ::LoadingWidget {
    friend singleton_t;
    LoadingWidget(): ::LoadingWidget(0, 0, zIndex) {
        auto holder = std::shared_ptr<::LoadingWidget>(this, [](auto){});
        Core::get()->add(shared_from_this());
    }

    static const int zIndex;
};
}

