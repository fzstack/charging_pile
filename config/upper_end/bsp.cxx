/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-02-03     imgcr       the first version
 */

#ifdef UPPER_END

#include <rtthread.h>
#include <board.h>
#include <devices/ws2812.hxx>
#include <devices/local_wtn6.hxx>
#include <components/packet.hxx>

namespace Preset {
const rt_base_t Ws2812::kDinPin = GET_PIN(B, 1);
const int Ws2812::kNum = 10;

const rt_base_t
    LocalWtn6::kDataPin = GET_PIN(C, 8),
    LocalWtn6::kBusyPin = GET_PIN(C, 7);

const char *Packet::kUart = "uart3";
}
#endif
