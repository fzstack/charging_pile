#pragma once

#include "screen.hxx"
#include <devices/ws2812.hxx>
#include <memory>
#include <components/timer.hxx>

class WsScreen: public Screen {
public:
    WsScreen(std::shared_ptr<Ws2812> ws2812, std::shared_ptr<Timer> timer, int width, int height);
    virtual int getWidth() override;
    virtual int getHeight() override;
    virtual Colors::Rgb getPixel(int x, int y) override;
    virtual void setPixel(Colors::Rgb color, int x, int y) override;
    virtual void flush() override;

protected:
    virtual int mapPixelIdx(int x, int y) = 0;

private:
    std::shared_ptr<Ws2812> ws2812;
    std::shared_ptr<Timer> timer;
    const int width, height;
    volatile bool willFlush = false;
};

#include <utilities/singleton.hxx>
namespace Preset {
class WsScreen: public Singleton<WsScreen>, public ::WsScreen {
    friend singleton_t;
    WsScreen(): ::WsScreen(Ws2812::get(), std::make_shared<Timer>(kDuration, kTimer), kWidth, kHeight) {}

protected:
    virtual int mapPixelIdx(int x, int y) {
        return x + Ws2812::kNum / kHeight * y;
    }

private:
    static const int kDuration;
    static const char* kTimer;

public:
    static const int kWidth, kHeight;
};
}



