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

    struct ChargerSpec {
        std::shared_ptr<Timer> timer;
        rt_tick_t tick;
        int prevCurr = 0;
        int prevVol = 0;
    };

    void update(ChargerInfo& info, ChargerSpec& spec);

    Observable<bool> inited = false;
    std::array<ChargerSpec, Config::Bsp::kPortNum> specs;
    rtthread::Mutex mutex;

    static constexpr int kDuration = 1000;
    static const char* kTimer;
    static const char* kMutex;
};
}

