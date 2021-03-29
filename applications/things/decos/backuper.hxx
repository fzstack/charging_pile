#pragma once

#include "base.hxx"
#include <utilities/observable.hxx>
#include <config/bsp.hxx>
#include <components/timer.hxx>

namespace Things::Decos {
/**
 * @description: 状态备份功能类
 */
class Backuper: public Base {
    friend outer_t;
    Backuper(outer_t* outer);
    virtual void init() override;

    Observable<bool> inited = false;

public:
    template<int N>
    struct Backup {
        int leftSeconds = 0;
        int timerId = 0;
        float consumption = 0;
    };

private:
    struct PortSpec {
        bool stateHasTransitioned = false;
    };

    Timer timer = {kDuration, kTimer};
    rt_uint8_t currPort = 0;
    std::array<PortSpec, Config::Bsp::kPortNum> specs;
    static constexpr int kDuration = 1000;
    static const char* kTimer;

};
}

