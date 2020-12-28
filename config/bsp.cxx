/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-12-12     imgcr       the first version
 */

#include <board.h>
#include <devices/rgb_light.hxx>
#include <devices/rc522.hxx>
#include <devices/relay.hxx>
#include <devices/wtn6.hxx>
#include <devices/load_detector.hxx>
#include <devices/hlw8112.hxx>
#include <devices/air724.hxx>

namespace Preset {
template<> const rt_base_t RgbLight<0>::kRPin = GET_PIN(B, 13);
template<> const rt_base_t RgbLight<0>::kGPin = GET_PIN(B, 14);
template<> const rt_base_t RgbLight<0>::kBPin = GET_PIN(B, 15);
template<> const rt_base_t RgbLight<1>::kRPin = GET_PIN(B, 10);
template<> const rt_base_t RgbLight<1>::kGPin = GET_PIN(B, 11);
template<> const rt_base_t RgbLight<1>::kBPin = GET_PIN(B, 12);

const char
    *Rc522::kSpiBus = "spi1",
    *Rc522::kSpiDev = "spi10";
const GPIO_TypeDef* Rc522::kSsGpioX = GPIOA;
const rt_uint16_t Rc522::kSsGpioPin = GPIO_PIN_4;

template<> const rt_base_t Relay<0>::kPin = GET_PIN(B, 2);
template<> const rt_base_t Relay<1>::kPin = GET_PIN(B, 7);

const rt_base_t
    Wtn6::kDataPin = GET_PIN(C, 8),
    Wtn6::kBusyPin = GET_PIN(C, 7);

template<> const rt_base_t LoadDetector<0>::kPin = GET_PIN(B, 0);
template<> const rt_base_t LoadDetector<1>::kPin = GET_PIN(B, 5);

const char *Hlw8112::kUart = "uart5";

const char *Air724::kUart = "uart2";
const rt_base_t Air724::kResetPin = GET_PIN(C, 4);
}
