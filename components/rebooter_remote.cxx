/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-04-08     imgcr       the first version
 */

#include "rebooter_remote.hxx"
#include <rthw.h>
#include <config/co.hxx>

RebooterRemote::RebooterRemote(std::shared_ptr<Packet> packet) {
    packet->on<Packets::Reboot>([this](auto p) {
        reboot();
    });
}

void RebooterRemote::reboot() {
    rt_hw_cpu_reset();
}
