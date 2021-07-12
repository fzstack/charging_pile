/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-04-08     imgcr       the first version
 */

#include "rebooter_local.hxx"
#include <rthw.h>
#include <config/co.hxx>

RebooterLocal::RebooterLocal(std::shared_ptr<Air724> air724, std::shared_ptr<Packet> packet): air724(air724), packet(packet) {

}

void RebooterLocal::reboot() {
    rt_hw_cpu_reset();
}

void RebooterLocal::rebootAll() {
    air724->reset();
    packet->emit<Packets::Reboot>({});
    reboot();
}

void RebooterLocal::rebootModule(std::string_view module) {
    if(module == "upper") {
        rt_kprintf("upper rebooting");
        reboot();
    } else if(module == "lower") {
        rt_kprintf("lower rebooting");
        packet->emit<Packets::Reboot>({});
    }
}
