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
#include <utilities/cmd.hxx>

#define LOG_TAG "test.packet"
#define LOG_LVL LOG_LVL_DBG
#include <ulog.h>

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

static void test_packet_a(int argc, char** argv) {
    ASSERT_MIN_NARGS(2);
    auto packet = Preset::Packet::get();
    auto a = atoi(argv[1]);
    packet->emit<PacketA>({a});
}

static void test_packet_control(int argc, char** argv) {
    ASSERT_MIN_NARGS(4);
    auto packet = Preset::Packet::get();
    packet->emit<Packets::Control>({
        port: atoi(argv[1]),
        timerId: atoi(argv[2]),
        minutes: atoi(argv[3]),
    });
}

static int init_test_packet() {
    auto packet = Preset::Packet::get();
    packet->on<PacketA>([](auto p) {
        rt_kprintf("p.a = %d\n", p->a);
    });

    packet->on<Packets::Control>([](auto p) {
       rt_kprintf("port: %d, timerId: %d, minutes: %d\n", p->port, p->timerId, p->minutes);
    });

    return RT_EOK;
}

INIT_APP_EXPORT(init_test_packet);
MSH_CMD_EXPORT(test_packet_a, );
MSH_CMD_EXPORT(test_packet_control, );
#endif
