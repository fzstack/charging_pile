/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-02-04     imgcr       the first version
 */

#include "remote_wtn6.hxx"
#include <config/co.hxx>

RemoteWtn6::RemoteWtn6(std::shared_ptr<Packet> packet): packet(packet) {

}

void RemoteWtn6::write(rt_uint8_t data) {
    packet->emit<Packets::Devices::Wtn6Write>({data});
}

