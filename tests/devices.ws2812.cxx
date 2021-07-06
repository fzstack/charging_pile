#include <devices/ws2812.hxx>
#include <rtthread.h>
#include <utilities/cmd.hxx>
#include <map>
#include <string>
#include <functional>
#include <components/timer.hxx>
#include <cmath>
#include <limits>

#define LOG_TAG "test.ws2812"
#define LOG_LVL LOG_LVL_DBG
#include <ulog.h>

using namespace std;

void test_ws2812_at(int argc, char** argv) {
    ASSERT_MIN_NARGS(2);
    auto ws2812 = Preset::Ws2812::get();
    auto sess = ws2812->getSess();
    int cur = atoi(argv[1]);
    for(auto i = 0; i < 10; i++) {
        if(i == cur) {
            ws2812->getPixel(i) = Colors::Rgb{r: 255, g: 255, b: 255};
        } else {
            ws2812->getPixel(i) = Colors::Rgb{r: 0, g: 0, b: 0};
        }
    }
}

class AniMode {
public:
    virtual void update(std::shared_ptr<Ws2812> ws) = 0;

    static constexpr int kTMiS = 20;
    static constexpr int kNLights = 10;
};


//二秒周期
//二秒 = 100次
//2pi -> 100

class BreathMode: public AniMode {
public:
    virtual void update(std::shared_ptr<Ws2812> ws) override {
        for(auto i = 0u; i < kNLights; i++) {
            auto bri = (rt_uint8_t)((sin(t * 2 * 3.1415926 / kNMax) + 1) * (numeric_limits<rt_uint8_t>::max() / 2.));
            ws->getPixel(i) = Colors::Rgb{bri, bri, bri};
        }
        t++;
        t %= kNMax;
    }

private:
    int t = 0;
    static constexpr int kTAniMiS = 2000;
    static constexpr int kNMax = kTAniMiS / kTMiS;
};

class RunMode: public AniMode {
    virtual void update(std::shared_ptr<Ws2812> ws) override {
        for(auto i = 0u; i < kNLights; i++) {
            auto curPixel = (tarPixel - i + kNLights) % kNLights;
            auto phase = kNPhasePer * i / 100. + (i != 0) * ((float)t / kNPerPixel) * (kNPhasePer / 100.f);
            if(phase >= 1) {
                phase = 1;
            }
            auto bri = rt_uint8_t(255 * lightFunc(phase));
            ws->getPixel(mapPos(curPixel)) = Colors::Rgb{bri, bri, bri};
        }
        t++;
        t %= kNPerPixel;
        if(t == 0) {
            tarPixel++;
            tarPixel %= kNLights;
        }
    }
    static constexpr int mapPos(int i) {
        return i < kNLights / 2 ? i : (kNLights * 3 / 2 - 1) - i;
    }
    template<typename T>
    T saturate(T val, T min, T max) {
        return std::min(std::max(val, min), max);
    }
    //i的值域为0..1, 定义域为0..1
    float lightFunc(float i) {
        float A = 1.01191877309523;
        float B = 0.566395890086378;
        float C = 0.612439088380144;
        float D = -0.766547296536989;
        return saturate((A - D) / (1 + pow(i / C, B)) + D, 0.f, 1.f);
    }

    int tarPixel = 0; //已映射的目标像素
    int t = 0;
    static constexpr int kTAniMiS = 1500;
    static constexpr int kNTotal = kTAniMiS / kTMiS; //一个周期内的帧数
    static constexpr int kNPerPixel = kNTotal / kNLights; //单个像素的帧数
    static constexpr int kNPhasePer = 27; //相位差, 以kNPerPixel的百分比为单位
};



class AniTimer: public Timer {
public:
    enum class Mode {
        Breath,
        Run,
    };

    AniTimer(): Timer(AniMode::kTMiS, "ani") {
        modes[Mode::Breath] = make_shared<BreathMode>();
        modes[Mode::Run] = make_shared<RunMode>();
    }

    void run() override {
        auto ws2812 = Preset::Ws2812::get();
        auto sess = ws2812->getSess();
        modes[mode]->update(ws2812);
    }

    void setMode(Mode mode) {
        this->mode = mode;
    }

private:
    int t = 0;
    Mode mode = Mode::Run;
    map<Mode, std::shared_ptr<AniMode>> modes;
};

std::shared_ptr<AniTimer> timer;

void test_ws2812_animate(int argc, char** argv) {
    ASSERT_MIN_NARGS(2);

    auto cmd = map<string, function<void()>>{
        {"start", []{
            timer->start();
        }},
        {"stop", []{
            timer->stop();
        }},
        {"mode", [&]{
            ASSERT_MIN_NARGS(3);
            auto modes = map<string, AniTimer::Mode> {
                {"breath", AniTimer::Mode::Breath},
                {"run", AniTimer::Mode::Run},
            };
            auto found = modes.find(argv[2]);
            ASSERT_ARG(mode, found != modes.end());
            timer->setMode(found->second);
        }},
    };

    auto found = cmd.find(argv[1]);
    ASSERT_ARG(cmd, found != cmd.end());
    found->second();
}

int init_test_ws2812() {
    timer = make_shared<AniTimer>();
    return RT_EOK;
}

MSH_CMD_EXPORT(test_ws2812_at, );
MSH_CMD_EXPORT(test_ws2812_animate, );
INIT_APP_EXPORT(init_test_ws2812);
