#pragma once

#include "base.hxx"
#include <utilities/observable.hxx>
#include <config/bsp.hxx>
#include <components/timer.hxx>
#include <utilities/count_down.hxx>
#include <components/lower/lower_backup.hxx>

namespace Things::Decos {
/**
 * @description: 状态备份功能类
 */
class Backuper: public Base {
    friend outer_t;
    Backuper(outer_t* outer);
    virtual void init() override;
    virtual void onStateChanged(InnerPort port, State::Value state) override;
    void resume(InnerPort port);

private:
    struct PortSpec {
        bool stateHasTransitioned = false;
        CountDown<> backupCount = {kBackupCnt};
        CountDown<> fResume = {};
    };

    std::shared_ptr<LowerBackup> man = Preset::LowerBackup::get();
    Timer timer = {kDuration, kTimer};
    rt_uint8_t currResumePort = 0;
    rt_uint8_t currBackupPort = 0;
    std::array<PortSpec, Config::Bsp::kPortNum> specs;
    //CountDown<> backupCount = {kBackupCnt};
    CountDown<> nextCount = {kNextCnt};

    static constexpr int kAutoBackupDur = 60 * 1000;
    static constexpr int kNextPortDur = 3 * 1000;
    static constexpr int kDuration = 100;

    static constexpr int kNextCnt = kNextPortDur / kDuration; //没计数这么多次触发下一个端口的备份
    static constexpr int kBackupCnt = kAutoBackupDur / kNextCnt / kDuration; //没计数这么多次触发一次备份
//    static constexpr int kDuration = kAutoBackupDur / kCnt / Config::Bsp::kPortNum;
    static const char* kTimer;

};
}

