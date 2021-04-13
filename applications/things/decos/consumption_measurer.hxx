#pragma once

#include "base.hxx"
#include <utilities/observable.hxx>
#include <components/timer.hxx>
#include <rtthread.h>
#include <array>
#include <config/bsp.hxx>
#include <Mutex.h>

namespace Things::Decos {
/**
 * @description: 功耗测量功能类
 */
class ConsumptionMeasurer: public Base {
    friend outer_t;
    ConsumptionMeasurer(outer_t* outer);
    virtual void init() override;
    virtual void onStateChanged(InnerPort port, State::Value state) override;
    virtual void onCurrentChanged(InnerPort port, int value) override;
    virtual void onVoltageChanged(InnerPort port, int value) override;

    struct ChargerSpec {
            rt_tick_t tick;
            int prevCurr = 0;
            int prevVol = 0;
        };

    void update(ChargerInfo& info, ChargerSpec& spec);

    bool isFirstCharge = true;

    std::array<ChargerSpec, Config::Bsp::kPortNum> specs;
    Timer timer = {kDuration, kTimer};

    static constexpr int kDuration = 1000;
    static const char* kTimer;
};
}

