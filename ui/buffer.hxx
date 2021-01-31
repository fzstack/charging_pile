/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-01-30     imgcr       the first version
 */
#pragma once

#include "colors.hxx"
#include <vector>
#include <memory>

class Buffer {
public:
    Buffer(int width, int height);
    void fill(Colors::Argb color = Colors::Argb::kTransparent);
    void fill(int x, int y, int width, int height, Colors::Argb color = Colors::Argb::kTransparent);
    void blend(int selfX, int selfY, int otherX, int otherY, std::shared_ptr<Buffer> other, int width, int height);
    std::vector<std::vector<Colors::Argb>> buf;

    const int width, height;
};


