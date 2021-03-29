#pragma once

#include "base.hxx"
#include <utilities/observable.hxx>
#include <components/timer.hxx>
#include <array>
#include <config/bsp.hxx>
#include <utilities/count_down.hxx>

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
        CountDown<> count = {kNoloadDurThr / kDuration};
    };

    Timer timer = {kDuration, kTimer};
    Observable<bool> inited = false;
    std::array<ChargerSpec, Config::Bsp::kPortNum> specs;
    static const char* kTimer;
    static constexpr int kDuration = 1000;
    static constexpr int kNoloadDurThr = 2 * 60 * 1000;  //空载时长阈值
};
}

