/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-03-24     imgcr       the first version
 */

#include "inner_port.hxx"
#include "nat_port.hxx"

InnerPort::InnerPort(): Port(0) {

}

InnerPort::InnerPort(rt_uint8_t value): Port(value) {

}

InnerPort::operator NatPort() {
    return NatPort(value + 1);
}
