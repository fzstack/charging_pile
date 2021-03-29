#pragma once

#include "base.hxx"
#include <utilities/observable.hxx>
#include <array>
#include <components/timer.hxx>
#include <config/bsp.hxx>
#include <Mutex.h>
#include <utilities/count_down.hxx>

namespace Things::Decos {
/**
 * @description: 限流功能类
 */
class CurrentLimiter: public Base {
    friend outer_t;
    CurrentLimiter(outer_t* outer);
    virtual void init() override;
    virtual void config(int currentLimit, int uploadThr, int fuzedThr, int noloadCurrThr) override;

public:
    struct Params {
        int maxCurrentMiA = 150; //电流变化阈值
    };


private:
    Observable<bool> inited = false;
    Timer timer = {kDuration, kTimer};
    std::array<CountDown<>, Config::Bsp::kPortNum> specs = {kInitCount,kInitCount,kInitCount,kInitCount,kInitCount,kInitCount,kInitCount,kInitCount,kInitCount,kInitCount};

    rtthread::Mutex mutex;

private:
    static constexpr int kDuration = 200;
    static constexpr int kInitCount = 10;
    static const char* kTimer;
    static const char* kMutex;
};
}

