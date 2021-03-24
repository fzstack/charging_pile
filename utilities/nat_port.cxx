/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-03-24     imgcr       the first version
 */

#include "nat_port.hxx"
#include "inner_port.hxx"

NatPort::NatPort(): Port(1) {

}

NatPort::NatPort(rt_uint8_t value): Port(value) {

}

NatPort::operator InnerPort() {
    return InnerPort(value - 1);
}

