/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-02-02     imgcr       the first version
 */
#pragma once
#include <rtdevice.h>

struct pin_index
{
    int index;
    GPIO_TypeDef *gpio;
    uint32_t pin;
};
extern const struct pin_index pins[];


