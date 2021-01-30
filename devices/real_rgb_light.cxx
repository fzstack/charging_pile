/*
#include <devices/real_rgb_light.hxx>
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-08-26     imgcr       the first version
 */

#include <rtdevice.h>

#include <utilities/observable.hxx>
#include <string>
#include <map>
#include <functional>
#include <optional>
#include "real_rgb_light.hxx"

#include <utilities/cmd.hxx>

using namespace std;
using namespace placeholders;

RealRgbLight::RealRgbLight(rt_base_t rPinId, rt_base_t gPinId, rt_base_t bPinId):
  rPin(rPinId), gPin(gPinId), bPin(bPinId) {
    r = rPin.getHandler();
    g = gPin.getHandler();
    b = bPin.getHandler();
}

