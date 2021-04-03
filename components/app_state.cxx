/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-04-01     imgcr       the first version
 */

#include "app_state.hxx"
#include <config/bsp.hxx>
#include <config/co.hxx>

AppState::AppState(std::shared_ptr<Packet> packet) {
    packet->on<Packets::State>([this](auto p) {
        portStates[p->port.get()].state = p->value;
        portStateChanged(p->port, p->value);
    });
}

State::Value AppState::getPortState(InnerPort port) {
    return portStates[port.get()].state;
}
