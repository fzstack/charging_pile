/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-04-02     imgcr       the first version
 */

#include "local_backup_man.hxx"
#include <components/persistent_storage_preset.hxx>
#include <config/co.hxx>
#include <Lock.h>

LocalBackupMan::LocalBackupMan(std::shared_ptr<Packet> packet, std::shared_ptr<Rpc> rpc, std::shared_ptr<SharedThread> thread):
conf(Preset::PersistentStorage::get()->read<RealBackupConf>()){
    auto storage = Preset::PersistentStorage::get();
    packet->on<Packets::BackupMan::Write>([this](auto p) {
        auto guard = rtthread::Lock(mutex);
        conf.backups[p->port.get()] = p->backup;
        fBackup.retrigger();
    });

    rpc->def<Rpcs::BackupMan::Read>([this](auto p){
        return conf.backups[p->port.get()];
    });

    timer.onRun += [this, storage, thread]{
        auto willWrite = false;
        {
            auto guard = rtthread::Lock(mutex);
            willWrite = fBackup.updateAndCheck();
        }
        if(willWrite) {
            thread->exec([this, storage](){
                storage->write(conf);
            });
        }
    };

    timer.start();
}
