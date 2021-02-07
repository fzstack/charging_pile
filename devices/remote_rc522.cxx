/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-02-05     imgcr       the first version
 */

#include "remote_rc522.hxx"
#include <config/co.hxx>
#include <string>

using namespace std;
using namespace Packets;

RemoteRc522::RemoteRc522(std::shared_ptr<Packet> packet): packet(packet) {
    packet->on<Devices::Rc522>([this](auto p) {
        cardId = p->cardId;
    });
}

