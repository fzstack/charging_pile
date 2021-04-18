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
#include <stdexcept>

using namespace std;

OtaLower::OtaLower(std::shared_ptr<Fal> fal, std::shared_ptr<Rpc> rpc, std::shared_ptr<SharedThread> thread): fal(fal), rpc(rpc), thread(thread), parti(fal_partition_find(kPartiName)) {
    if(parti == nullptr) {
        rt_kprintf("fal parti %s not found\n", kPartiName);
        throw not_implemented{"ota parti not found"};
    }

    rpc->def<Rpcs::Ota::Erase>([this](auto p, auto r) {
        this->thread->exec([=]{
            rt_kprintf("erasing..., size: %d\n", p->size);
            auto res = fal_partition_erase(parti, 0, p->size);
            if(res < 0) {
                rt_kprintf("parti erase failed\n");
                r(make_exception_ptr(runtime_error{"parti erase failed"}));
            } else {
                rt_kprintf("erased\n");
                r(Void{});
            }
        });
    });

    rpc->def<Rpcs::Ota::Write>([this](auto p, auto r) {
        this->thread->exec([=]{
            rt_kprintf("%d\n", p->addr);
            fal_partition_write(parti, p->addr, &p->data[0], p->data.size());
//            if(res < 0) {
//                r(make_exception_ptr(runtime_error{"write err"}));
//                return;
//            }
            r(Void{});
        });
    });

    rpc->def<Rpcs::Ota::GetVersion>([](auto p){
        return ::moduleVersion;
    });
}

const char* OtaLower::kPartiName = "download";
