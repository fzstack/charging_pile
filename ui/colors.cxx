/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-01-31     imgcr       the first version
 */

#include "colors.hxx"

using namespace Colors;

Argb Rgb::toArgb(rt_uint8_t alpha) {
    return {alpha, r, g, b};
}

const Rgb Rgb::kBlack = {0, 0, 0};

const Argb
    Argb::kTransparent = {0, 0, 0, 0},
    Argb::kBlack = {255, 0, 0, 0};


