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
#include "buffer.hxx"
#include <memory>

class Graphics {
public:
    Graphics(std::shared_ptr<Buffer> buffer);
    void fillRect(Colors::Argb color, int x, int y, int width, int height);
    void setPixel(Colors::Argb color, int x, int y);
    void addPixel(Colors::Argb color, int x, int y);
private:
    std::shared_ptr<Buffer> buffer;
};


