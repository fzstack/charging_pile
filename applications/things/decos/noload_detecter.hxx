#pragma once

#include "base.hxx"
#include <utilities/observable.hxx>
#include <components/timer.hxx>
#include <array>
#include <config/bsp.hxx>

namespace Things::Decos {
/**
 * @description: 空载检测功能类
 */
class NoloadDetecter: public Base {
    friend outer_t;
    NoloadDetecter(outer_t* outer);
    virtual void init() override;
    virtual void config(int currentLimit, int uploadThr, int fuzedThr, int noloadCurrThr) override;

public:
    struct Params {
        int noloadCurrThr = 10; //空载电流阈值
    };

private:
    struct ChargerSpec {
        Timer timer = {kDuration, kTimer, RT_TIMER_FLAG_ONE_SHOT};
    };

    Observable<bool> inited = false;
    std::array<ChargerSpec, Config::Bsp::kPortNum> specs;
    static const char* kTimer;
    static constexpr int kDuration = 4000; //空载判断区间
};
}

