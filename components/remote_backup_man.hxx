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
#include <components/packet.hxx>
#include <components/rpc.hxx>

class RemoteBackupMan: public BackupMan {
public:
    RemoteBackupMan(std::shared_ptr<Packet> packet, std::shared_ptr<Rpc> rpc);
    virtual void read(InnerPort port, std::function<void(std::optional<Backup>)>) override;
    virtual void write(InnerPort port, Backup backup) override;

private:
    std::shared_ptr<Packet> packet;
    std::shared_ptr<Rpc> rpc;
};

#include <utilities/singleton.hxx>
namespace Preset {
class RemoteBackupMan: public Singleton<RemoteBackupMan>, public ::RemoteBackupMan {
    friend singleton_t;
    RemoteBackupMan(): ::RemoteBackupMan(Packet::get(), Rpc::get()) { }
};
}
