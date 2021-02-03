/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-01-30     imgcr       the first version
 */

#include "graphics.hxx"

Graphics::Graphics(std::shared_ptr<Buffer> buffer): buffer(buffer) {

}

void Graphics::fillRect(Colors::Argb color, int x, int y, int width, int height) {
    for(auto i = x; i < x + width; i++) {
        for(auto j = y; j < y + height; j++) {
            buffer->get(i, j) = color;
        }
    }
}

void Graphics::setPixel(Colors::Argb color, int x, int y) {
    buffer->get(x, y) = color;
}

void Graphics::addPixel(Colors::Argb color, int x, int y) {
    buffer->get(x, y) += color;
}
