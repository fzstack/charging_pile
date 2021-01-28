/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-01-27     imgcr       the first version
 */

#ifdef TEST_PACKET

#include <components/packet.hxx>
#include <rtthread.h>

struct PacketA {
    int a;
};

namespace Packets {
struct Control {
    int port;
    int timerId;
    int minutes;
};
}

static void test_packet() {
    auto packet = Preset::Packet::get();
//    packet->on<PacketA>([](auto p) {
//        rt_kprintf("p.a = %d\n", p->a);
//    });
//
//    packet->on<Packets::Control>([](auto p) {
//       rt_kprintf("port: %d, timerId: %d, minutes: %d\n", p->port, p->timerId, p->minutes);
//    });

    packet->emit<PacketA>(PacketA{4});
}

MSH_CMD_EXPORT(test_packet, );

#endif
