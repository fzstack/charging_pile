/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-04-02     imgcr       the first version
 */

#include "remote_backup_man.hxx"
#include <config/co.hxx>


RemoteBackupMan::RemoteBackupMan(std::shared_ptr<Packet> packet, std::shared_ptr<Rpc> rpc): packet(packet), rpc(rpc) {

}

void RemoteBackupMan::read(InnerPort port, std::function<void(std::optional<Backup>)> cb) {
    rpc->invoke<Rpcs::BackupMan::Read>({port}, [cb](auto r){
        auto pdata = std::get_if<Backup>(&r);
        if(pdata == nullptr) {
            cb(std::nullopt);
        } else {
            cb(*pdata);
        }
    });
}

void RemoteBackupMan::write(InnerPort port, Backup backup) {
    packet->emit<Packets::BackupMan::Write>({port, backup});
}

