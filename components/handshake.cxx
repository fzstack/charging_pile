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

//在接受到第一次responsed的时候，下次发送一个带done req的handshake出去

Handshake::Handshake(std::shared_ptr<Packet> packet): packet(packet) {
    packet->on<Packets::Handshake>([this](auto p){
        responsed = true;
        if(p->sync) {
            connected = false;
            connected = true;
        }
    });

    timer.onRun += [this]{
        auto cond = !once && responsed;
        if(cond) once = true;
        this->packet->emit<Packets::Handshake>({cond});
    };
}

void Handshake::hello() {
    timer.start();
    while(!responsed) {
        rt_thread_mdelay(100);
    }
}

namespace Preset {
Handshake::Handshake(): ::Handshake(Packet::get()) {

}
}