/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-03-24     imgcr       the first version
 */
#pragma once
#include <rtthread.h>
#include "port.hxx"

class NatPort;
class InnerPort: public Port {
public:
    InnerPort();
    explicit InnerPort(rt_uint8_t value);
    operator NatPort();
};


