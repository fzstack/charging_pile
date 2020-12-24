#include <config/bsp.hxx>
/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-12-12     imgcr       the first version
 */



using namespace Config;

const char
    *Bsp::kHlw8112Uart = "uart5",
    *Bsp::kRc522SpiBus = "spi1",
    *Bsp::kRc522SpiDev = "spi10",
    *Bsp::kAir202Uart = "uart2";

const GPIO_TypeDef* Bsp::kRc522GpioX = GPIOA;
