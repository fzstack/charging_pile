#pragma once

#include "base.hxx"
#include <utilities/observable.hxx>
#include <config/bsp.hxx>
#include <components/timer.hxx>
#include <utilities/count_down.hxx>
#include <components/backup_man_preset.hxx>

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
        CountDown<> count = {kCnt};
        CountDown<> fResume = {};
    };

    std::shared_ptr<BackupMan> man = Preset::BackupMan::get();
    Timer timer = {kDuration, kTimer};
    rt_uint8_t currResumePort = 0;
    rt_uint8_t currBackupPort = 0;
    std::array<PortSpec, Config::Bsp::kPortNum> specs;
    static constexpr int kCnt = 150;
    static constexpr int kAutoBackupDur = 1800 * 1000;
    static constexpr int kDuration = kAutoBackupDur / kCnt / Config::Bsp::kPortNum;
    static const char* kTimer;

};
}

