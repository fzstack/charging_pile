#pragma once

#include "base.hxx"
#include <utilities/observable.hxx>
#include <config/bsp.hxx>
#include <components/timer.hxx>
#include <utilities/count_down.hxx>

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
        CountDown<> count = {kCnt};
    };

    Timer timer = {kDuration, kTimer};
    rt_uint8_t currPort = 0;
    std::array<PortSpec, Config::Bsp::kPortNum> specs;
    static constexpr int kCnt = 10;
    static constexpr int kAutoBackupDur = 10 * 1000;
    static constexpr int kDuration = kAutoBackupDur / kCnt / Config::Bsp::kPortNum;
    static const char* kTimer;

};
}

