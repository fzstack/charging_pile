/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-02-04     imgcr       the first version
 */

#include "local_wtn6.hxx"

LocalWtn6::LocalWtn6(rt_base_t dataPin, rt_base_t busyPin, std::shared_ptr<Packet> packet): Wtn6(dataPin, busyPin), packet(packet) {
    packet->on<Packets::Devices::Wtn6Write>([this](auto p){
        this->write(p->value);
    });
}

