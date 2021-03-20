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
#include <devices/ttp229.hxx>
#include <devices/local_rc522.hxx>
#include <devices/air724.hxx>
#include <components/packet.hxx>
#include <components/fal_persistent_storage.hxx>

namespace Preset {
const rt_base_t Ws2812::kDinPin = GET_PIN(B, 1);
const int Ws2812::kNum = 10;

const rt_base_t
    LocalWtn6::kDataPin = GET_PIN(C, 8),
    LocalWtn6::kBusyPin = GET_PIN(C, 7);

const rt_base_t
    Ttp229::kSclPin = GET_PIN(C, 2),
    Ttp229::kSdoPin = GET_PIN(C, 3);

const char
    *LocalRc522::kSpiBus = "spi1",
    *LocalRc522::kSpiDev = "spi10";
const rt_base_t LocalRc522::kSsPin = GET_PIN(A, 4);

const char *Air724::kUart = "uart2";
const rt_base_t Air724::kResetPin = GET_PIN(A, 1);

const char *Packet::kUart = "uart3";

const char
    *FalPersistentStorage::kSpiBus = "spi2",
    *FalPersistentStorage::kSpiDev = "spi20";
const rt_base_t FalPersistentStorage::kSsPin = GET_PIN(B, 12);
}




#endif
