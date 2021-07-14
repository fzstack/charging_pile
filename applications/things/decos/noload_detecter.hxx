#pragma once

#include "base.hxx"
#include <utilities/observable.hxx>
#include <components/timer.hxx>
#include <array>
#include <config/bsp.hxx>
#include <utilities/count_down.hxx>
#include <optional>

namespace Things::Decos {
/**
 * @description: 空载检测功能类
 */
class NoloadDetecter: public Base {
    friend outer_t;
    NoloadDetecter(outer_t* outer);
    virtual void init() override;
    virtual void onStateChanged(InnerPort port, State::Value state) override;
    virtual void onCurrentChanged(InnerPort port, int value) override;

private:
    struct ChargerSpec {
        CountDown<> noloadCount = {kNoloadDurThr / kDuration / Config::Bsp::kPortNum};
        CountDown<> doneCount = {kDoneDurThr / kDuration / Config::Bsp::kPortNum};
    };

    void checkCurrent(InnerPort port, int value);

    Timer timer = {kDuration, kTimer};
    Observable<bool> inited = false;
    std::array<ChargerSpec, Config::Bsp::kPortNum> specs;
    rt_uint8_t currPort = 0;
    static const char* kTimer;
    static constexpr int kDuration = 100;
    static constexpr int kNoloadDurThr = 2 * 60 * 1000;  //空载时长阈值
    static constexpr int kDoneDurThr = (2 * 60 + 5) * 1000;  //充电完成时长阈值
};
}

