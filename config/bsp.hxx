/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-12-12     imgcr       the first version
 */
#ifndef CONFIG_BSP_CONFIG_HXX_
#define CONFIG_BSP_HXX_

#include <board.h>

namespace Config {
class Bsp {
public:
    static constexpr rt_base_t
        kWtn6DataPin = GET_PIN(C, 8),
        kWtn6BusyPin = GET_PIN(C, 7),
        kRelayAPin = GET_PIN(B, 2),
        kRelayBPin = GET_PIN(B, 7),
        kLoadDetectorAPin = GET_PIN(B, 0),
        kLoadDetectorBPin = GET_PIN(B, 5),
        kRgbLightARPin = GET_PIN(B, 13),
        kRgbLightAGPin = GET_PIN(B, 14),
        kRgbLightABPin = GET_PIN(B, 15),
        kRgbLightBRPin = GET_PIN(B, 10),
        kRgbLightBGPin = GET_PIN(B, 11),
        kRgbLightBBPin = GET_PIN(B, 12),
        kAir202RstPin = GET_PIN(C, 4);

    static const char
        *kHlw8112Uart,
        *kRc522SpiBus,
        *kRc522SpiDev,
        *kAir202Uart;

    static const GPIO_TypeDef* kRc522GpioX;
    static constexpr rt_uint16_t kRc522GpioPin = GPIO_PIN_4;
};
}



#endif /* CONFIG_BSP_CONFIG_HXX_ */
