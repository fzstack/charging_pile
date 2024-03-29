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
    virtual void onCurrentChanged(InnerPort port, int value) override;
private:
    Timer timer = {kDuration, kTimer};
    struct Spec {
        CountDown<> count = {kInitCount};
    };
    std::array<Spec, Config::Bsp::kPortNum> specs;
private:
    static constexpr int kDuration = 200;
    static constexpr int kInitCount = 10;
    static const char* kTimer;
};
}

