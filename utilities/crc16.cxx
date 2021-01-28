/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-01-28     imgcr       the first version
 */

#include "crc16.hxx"

void Crc16::reset() {
    val = CRC16_INIT_VAL;
}

void Crc16::update(uint8_t dat) {
    val = crc16_cyc_cal(val, &dat, sizeof(dat));
}

rt_uint16_t Crc16::get() {
    return val ^ CRC16_INIT_VAL;
}
