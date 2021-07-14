#pragma once

#include "base.hxx"
#include <utilities/observable.hxx>
#include <array>
#include <config/bsp.hxx>
#include <components/timer.hxx>
#include <utilities/count_down.hxx>

namespace Things::Decos {
/**
 * @description: 状态数据上报功能类
 */
class DataSetter: public Base {
    friend outer_t;
    DataSetter(outer_t* outer);
    virtual void init() override;
    virtual void query() override;
    virtual void onStateChanged(InnerPort port, State::Value state) override;
    virtual void onCurrentChanged(InnerPort port, int value) override;

private:
    void emitPortData(InnerPort port);

private:
    struct Spec {
        int prevCurrMiA = 0;
        State::Value prevState;
        float prevConsumption;
        CountDown<> count = {1};
    };

private:
    std::array<Spec, Config::Bsp::kPortNum> specs;

    Timer timer = {kDuration, kTimer};
    rt_uint8_t currPort = 0;
    static constexpr int kDuration = 100;
    static const char* kTimer;
};
}

