/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-02-02     imgcr       the first version
 */

#include "uart_hlw8112.hxx"

using namespace std;

UartHlw8112::UartHlw8112(const char* uartName): Hlw8112() {
    struct serial_configure conf = RT_SERIAL_CONFIG_DEFAULT;
    conf.data_bits = DATA_BITS_9;
    conf.baud_rate = BAUD_RATE_9600;
    conf.parity = PARITY_EVEN;
    uart = make_shared<QueuedUart>(uartName, &conf);
}

void UartHlw8112::cmd(int cmd, void* data, int len) {
    char cs = 0;
    uart->send<char>(0xa5);
    cs += 0xa5;

    uart->send<char>(cmd);
    cs += cmd;

    for(char* p = (char*)data + len - 1; p >= (char*)data; p--) {
        uart->send(p, 1);
        cs += *p;
    }

    cs = ~cs;
    uart->send(cs);
}

void UartHlw8112::readReg(char addr, void* data, int len, rt_int32_t timeout) {
    char cs_expect = 0, cs;
    uart->send<char>(0xa5);
    cs_expect += 0xa5;

    uart->send(addr);
    cs_expect += addr;

    for(char* p = (char*)data + len - 1; p >= (char*)data; p--) {
        uart->recv(p, 1, timeout);
        cs_expect += *p;
    }
    cs_expect = ~cs_expect;
    uart->recv(&cs, 1, timeout);

    if(cs != cs_expect) {
        uart->clear();
        throw hlw8112_error{"wrong checksum"};
    }
}
