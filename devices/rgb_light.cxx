/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-08-26     imgcr       the first version
 */

#include "rgb_light.hxx"
#include <rtdevice.h>

#include "observable.hxx"
#include <string>
#include <map>
#include <functional>
#include <optional>

#include <cmd.hxx>

using namespace std;
using namespace placeholders;

RgbLight::RgbLight(rt_base_t rPinId, rt_base_t gPinId, rt_base_t bPinId):
    rPin(rPinId), gPin(gPinId), bPin(bPinId),
    r(rPin), g(gPin), b(bPin) {

}

