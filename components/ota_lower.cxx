/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-04-15     imgcr       the first version
 */

#include "ota_lower.hxx"
#include <config/co.hxx>


OtaLower::OtaLower(std::shared_ptr<Fal> fal, std::shared_ptr<Rpc> rpc): fal(fal), rpc(rpc), parti(fal_partition_find(kPartiName)) {
    if(parti == nullptr) {
        rt_kprintf("fal parti %s not found\n", kPartiName);
        throw not_implemented{"ota parti not found"};
    }

    rpc->def<Rpcs::Ota::Erase>([this](auto p) {
        fal_partition_erase(parti, 0, p->size);
    });

    rpc->def<Rpcs::Ota::Write>([this](auto p) {
        fal_partition_write(parti, p->addr, &p->data[0], p->data.size());
    });

    rpc->def<Rpcs::Ota::GetVersion>([](auto p){
        return ::moduleVersion;
    });
}

const char* OtaLower::kPartiName = "download";
