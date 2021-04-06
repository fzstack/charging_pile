#pragma once

#include "widget.hxx"
#include <tuple>

class LoadingWidget: public CSizeWidget<5, 2> {
    using p_t = CSizeWidget<5, 2>;
public:
    LoadingWidget(int x, int y, int zIndex);
protected:
    virtual void onDraw(std::shared_ptr<Graphics> graphics) override;
    virtual void onTick() override;
private:
    float timing(float t);
    std::tuple<int, int> getPosFromIdx(int idx);
    static int particleNumFromSize(int width, int height);

    const int particleNum = particleNumFromSize(getWidth(), getHeight());
    const float phaseStep = 1.f / (particleNum);
    float curVal = 0;
    static constexpr int kDuration = 1700;
    static const Colors::Argb kBaseColor, kBackground;
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

