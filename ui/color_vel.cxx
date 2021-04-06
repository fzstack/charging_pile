/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-04-06     imgcr       the first version
 */

#include "color_vel.hxx"

float ColorVel::ease(float x) {
    return -x * x + 2 * x;
}

Colors::Argb ColorVel::intepl(const Colors::Argb& originVal, const Colors::Argb& targetVal, float pos) {
    return Colors::Argb{
        a: between(originVal.a, targetVal.a, pos),
        r: between(originVal.r, targetVal.r, pos),
        g: between(originVal.g, targetVal.g, pos),
        b: between(originVal.b, targetVal.b, pos),
    };
}
