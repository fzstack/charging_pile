/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-02-04     imgcr       the first version
 */
#pragma once

#include <rtthread.h>

class Wtn6Base {
public:
    virtual void write(rt_uint8_t data) = 0;

    template<class T>
    void write(T data) {
        write(rt_uint8_t(data));
    }
};


