/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-01-30     imgcr       the first version
 */

#include "buffer.hxx"
#include <algorithm>

using namespace std;

Buffer::Buffer(int width, int height): buf(width), width(width), height(height) {
    for(auto i = 0; i < width; i++) {
        buf[i] = vector<Colors::Argb>(height);
    }
}

void Buffer::fill(Colors::Argb color) {
    fill(0, 0, width, height, color);
}

void Buffer::fill(int x, int y, int width, int height, Colors::Argb color) {
    for(auto i = x; i < width; i++) {
        for(auto j = y; j < height; j++) {
            buf[i][j] = color;
        }
    }
}

void Buffer::blend(int selfX, int selfY, int otherX, int otherY, shared_ptr<Buffer> other, int width, int height) {
//    auto w = min({this->width - selfX, other->width - otherX, width});
//    auto h = min({this->height - selfY, other->height - otherY, height});
//
//    //自己的一块区域和另一个缓存的另一块区域混合
//    if(w <= 0 || h <= 0 || selfX < 0 || selfY < 0 || otherX < 0 || otherY < 0) return;
//    for(auto dx = 0; dx < w; dx++) {
//        for(auto dy = 0; dy < h; dy++) {
//            this->buf[selfX + dx][selfY + dy] += other->buf[otherX + dx][otherY + dy];
//        }
//    }
    for(auto dx = selfX; dx < selfX + width; dx++) {
        for(auto dy = selfY; dy < selfY + height; dy++) {
            if(dx >= otherX && dy >= otherY && dx < otherX + other->width && dy < otherY + other->height) {
                this->buf[dx][dy] += other->buf[dx - otherX][dy - otherY];
            }
        }
    }
}
