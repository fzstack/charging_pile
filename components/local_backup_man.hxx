/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-04-02     imgcr       the first version
 */
#pragma once

#include "backup_man.hxx"
#include <utilities/err.hxx>
#include <components/packet.hxx>
#include <components/rpc.hxx>
#include <array>
#include <config/bsp.hxx>
#include <components/timer.hxx>
#include <utilities/count_down.hxx>
#include <Mutex.h>
#include <utilities/shared_thread.hxx>


class LocalBackupMan: public BackupMan {
public:
    LocalBackupMan(std::shared_ptr<Packet> packet, std::shared_ptr<Rpc> rpc, std::shared_ptr<SharedThread> thread);

    virtual void read(InnerPort port, std::function<void(std::optional<Backup>)>) override {
        throw not_implemented{};
    }

    virtual void write(InnerPort port, Backup backup) override {
        throw not_implemented{};
    }

private:
    struct RealBackupConf {
        std::array<Backup, Config::Bsp::kPortNum> backups;
    };
    RealBackupConf conf;
    CountDown<> fBackup = {4};
    Timer timer = {500, "lbm"};
    rtthread::Mutex mutex = {"lbm"};
};

#include <utilities/singleton.hxx>
namespace Preset {
class LocalBackupMan: public Singleton<LocalBackupMan>, public ::LocalBackupMan {
    friend singleton_t;
    LocalBackupMan(): ::LocalBackupMan(Packet::get(), Rpc::get(), SharedThread<Priority::Middle>::get()) { }
};
}
