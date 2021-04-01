/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-04-01     imgcr       the first version
 */

#include "handshake.hxx"
#include <config/co.hxx>

Handshake::Handshake(std::shared_ptr<Packet> packet): packet(packet) {
    packet->on<Packets::Handshake>([this](auto p){
        responsed = true;
    });

    timer.onRun += [this]{
        this->packet->emit<Packets::Handshake>();
    };
}

void Handshake::hello() {
    timer.start();
    while(!responsed) {
        rt_thread_mdelay(100);
    }
    done();
}
