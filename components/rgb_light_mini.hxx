/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-02-09     imgcr       the first version
 */
#pragma once

#include <devices/rgb_light.hxx>
#include <devices/ws2812.hxx>
#include <memory>

class RgbLightMini: public RgbLight {
public:
    RgbLightMini(int i, std::shared_ptr<Ws2812> ws2812);

private:
    int i;
    std::shared_ptr<Ws2812> ws2812;
    static constexpr int kMax = 100;
};

#include <utilities/singleton.hxx>
namespace Preset {
template<int R>
class RgbLightMini: public Singleton<RgbLightMini<R>>, public ::RgbLightMini {
    friend Singleton<RgbLightMini<R>>;
    RgbLightMini(): ::RgbLightMini(R, Ws2812::get()) { }
};
}
