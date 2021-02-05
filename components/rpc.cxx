/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-02-05     imgcr       the first version
 */

#include "rpc.hxx"

Rpc::Rpc(std::shared_ptr<Packet> packet): packet(packet) {

}

