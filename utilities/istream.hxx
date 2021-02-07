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

class IStream {
public:
    template<class T>
    T read() {
        T t;
        readData((rt_uint8_t*)&t, sizeof(T));
        return t;
    }

    virtual void readData(rt_uint8_t* data, int len) = 0;
};


