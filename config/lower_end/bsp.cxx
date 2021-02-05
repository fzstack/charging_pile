/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-02-03     imgcr       the first version
 */

#ifdef LOWER_END
#include <board.h>
#include <devices/relay.hxx>
#include <devices/wtn6.hxx>
#include <devices/load_detector.hxx>
#include <devices/uart_hlw8112.hxx>
#include <devices/spi_hlw8112.hxx>
#include <components/persistent_storage.hxx>
#include <components/packet.hxx>
#include <devices/real_rgb_light.hxx>
#include <devices/ws2812.hxx>
#include <rtdevice.h>
#include <stm32f1xx.h>

namespace Preset {
template<> const char *SpiHlw8112<0>::kSpiBus = "spi1";
template<> const char *SpiHlw8112<0>::kSpiDev = "spi11";
template<> const rt_base_t SpiHlw8112<0>::kSsPin = GET_PIN(A, 0);

template<> const char *SpiHlw8112<1>::kSpiBus = "spi1";
template<> const char *SpiHlw8112<1>::kSpiDev = "spi12";
template<> const rt_base_t SpiHlw8112<1>::kSsPin = GET_PIN(B, 14);

template<> const char *SpiHlw8112<2>::kSpiBus = "spi1";
template<> const char *SpiHlw8112<2>::kSpiDev = "spi13";
template<> const rt_base_t SpiHlw8112<2>::kSsPin = GET_PIN(C, 3);

template<> const char *SpiHlw8112<3>::kSpiBus = "spi1";
template<> const char *SpiHlw8112<3>::kSpiDev = "spi14";
template<> const rt_base_t SpiHlw8112<3>::kSsPin = GET_PIN(A, 15);

template<> const char *SpiHlw8112<4>::kSpiBus = "spi1";
template<> const char *SpiHlw8112<4>::kSpiDev = "spi15";
template<> const rt_base_t SpiHlw8112<4>::kSsPin = GET_PIN(D, 2);

template<> const rt_base_t Relay<0>::kPin = GET_PIN(A, 2);
template<> const rt_base_t Relay<1>::kPin = GET_PIN(A, 3);
template<> const rt_base_t Relay<2>::kPin = GET_PIN(A, 4);
template<> const rt_base_t Relay<3>::kPin = GET_PIN(B, 15);
template<> const rt_base_t Relay<4>::kPin = GET_PIN(C, 6);
template<> const rt_base_t Relay<5>::kPin = GET_PIN(C, 9);
template<> const rt_base_t Relay<6>::kPin = GET_PIN(A, 8);
template<> const rt_base_t Relay<7>::kPin = GET_PIN(C, 10);
template<> const rt_base_t Relay<8>::kPin = GET_PIN(C, 2);
template<> const rt_base_t Relay<9>::kPin = GET_PIN(C, 1);

template<> const rt_base_t LoadDetector<0>::kPin = GET_PIN(B, 0);
template<> const rt_base_t LoadDetector<1>::kPin = GET_PIN(B, 1);
template<> const rt_base_t LoadDetector<2>::kPin = GET_PIN(B, 2);
template<> const rt_base_t LoadDetector<3>::kPin = GET_PIN(B, 3);
template<> const rt_base_t LoadDetector<4>::kPin = GET_PIN(B, 4);
template<> const rt_base_t LoadDetector<5>::kPin = GET_PIN(B, 5);
template<> const rt_base_t LoadDetector<6>::kPin = GET_PIN(B, 6);
template<> const rt_base_t LoadDetector<7>::kPin = GET_PIN(B, 7);
template<> const rt_base_t LoadDetector<8>::kPin = GET_PIN(B, 8);
template<> const rt_base_t LoadDetector<9>::kPin = GET_PIN(B, 9);

const char *UartHlw8112::kUart = "uart5";

const char* PersistentStorage::kI2c = "i2c1";
const int PersistentStorage::kAddr = 0;
const int PersistentStorage::kSize = 2048;

const char *Packet::kUart = "uart3";

const rt_base_t Ws2812::kDinPin = GET_PIN(C, 4);
//C4
const int Ws2812::kNum = 10;

}
#endif
