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

Handshake::Handshake(std::shared_ptr<Packet> packet, HandshakeType type): packet(packet), type(type) {
    rt_kprintf("handshake ctor! type: %d\n", type);
    packet->on<Packets::Handshake>([this](auto p){
        onHandshake(p->type);
        if(p->type != this->type) {
            return;
        }
        responsed = true;
        if(p->sync) {
            connected = false;
            connected = true;
        }
    });

    timer.onRun += [this]{
        auto cond = !once && responsed;
        if(cond) once = true;
        this->packet->emit<Packets::Handshake>({cond, this->type});
    };
}

void Handshake::hello() {
    timer.start();
    while(!responsed) {
        rt_thread_mdelay(100);
    }
}

namespace Preset {
Handshake::Handshake(): ::Handshake(Packet::get(), HandshakeType::Major) { 
    rt_kprintf("NEVER CALL THIS FUNC\n");
    RT_ASSERT(0);
}
Handshake::Handshake(HandshakeType type): ::Handshake(Packet::get(), type) { }
// OtaHandshake::OtaHandshake(): ::Handshake(Packet::get(), HandshakeType::Ota) { }
}