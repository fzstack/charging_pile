/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-08-26     imgcr       the first version
 */
#ifndef DEVICES_RGB_LIGHT_HXX_
#define DEVICES_RGB_LIGHT_HXX_

#include <rtthread.h>
#include <optional>
#include "output_pin.hxx"


class RgbLight {
public:
    RgbLight(rt_base_t rPinId, rt_base_t gPinId, rt_base_t bPinId);

private:
    OutputPin rPin, gPin, bPin;

public:
    OutputPin::handler_t &r, &g, &b;
};

#include <utilities/singleton.hxx>
#include <config/bsp.hxx>
namespace Preset{
template<int R>
class RgbLight: public Singleton<RgbLight<R>>, public Config::Bsp::assert_t<::RgbLight, R> {
    friend class Singleton<RgbLight>;
    RgbLight(): ::RgbLight(kRPin, kGPin, kBPin) {};
    static const rt_base_t kRPin, kGPin, kBPin;
};
}

#endif /* APPLICATIONS2_RGB_LIGHT_HXX_ */
