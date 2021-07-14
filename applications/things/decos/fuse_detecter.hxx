#pragma once

#include "base.hxx"
#include <utilities/observable.hxx>
#include <config/bsp.hxx>
#include <list>
#include <array>
#include <components/timer.hxx>

namespace Things::Decos {
/**
 * @description: 保险丝检测功能类
 */
class FuseDetecter: public Base {
    friend outer_t;
    FuseDetecter(outer_t* outer);
    virtual void init() override;

public:
    struct Params {
        int fuzedS2Thr = 10000000;
    };

private:
    struct ChargerSpec {
        std::list<float> angleHist = {};
    };

    static const char* kTimer;
    static constexpr int kDuration = 1000;
    static constexpr int kSatSize = 10;
    Timer timer = {kDuration, kTimer};
    std::array<ChargerSpec, Config::Bsp::kPortNum> specs;
    Observable<bool> inited = false;
};
}

