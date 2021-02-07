/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-02-07     imgcr       the first version
 */
#pragma once

#include <rtthread.h>

class OStream {
public:
    template<class T>
    void write(T&& t) {
        writeData((rt_uint8_t*)(void*)&t, sizeof(T));
    }

    virtual void writeData(rt_uint8_t* data, int len) = 0;
};

