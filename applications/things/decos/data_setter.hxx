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
    virtual void config(int currentLimit, int uploadThr, int fuzedThr, int noloadCurrThr) override;

private:
    void emitPortData(InnerPort port);

public:
    struct Params {
        int currDiffThrMiA = 10; //电流变化阈值
    };

private:
    struct ChargerSpec {
        int prevCurrMiA = 0;
        CountDown<> count = {1};
    };

private:
    std::array<ChargerSpec, Config::Bsp::kPortNum> specs;
    Observable<bool> inited = false;

    Timer timer = {kDuration, kTimer};
    rt_uint8_t currPort = 0;
    static constexpr int kDuration = 100;
    static const char* kTimer;
};
}

