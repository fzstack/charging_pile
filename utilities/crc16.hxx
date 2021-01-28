/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-01-28     imgcr       the first version
 */
#ifndef UTILITIES_CRC16_HXX_
#define UTILITIES_CRC16_HXX_

#include <rtthread.h>
extern "C" {
#include "crc16.h"
}

class Crc16 {
public:
    void reset();
    void update(uint8_t dat);
    rt_uint16_t get();

    rt_uint16_t val = CRC16_INIT_VAL;
};



#endif /* UTILITIES_CRC16_HXX_ */
