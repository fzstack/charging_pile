/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-03-20     imgcr       the first version
 */

#include "memory_iostream.hxx"
#include <algorithm>

using namespace std;

void MemoryIOStream::readData(rt_uint8_t* data, int len) {
    for(auto i = 0; i < len; i++) {
        data[i] = buf.front();
        buf.pop_front();
    }
}

void MemoryIOStream::writeData(rt_uint8_t* data, int len) {
    for(auto i = 0; i < len; i++) {
        buf.push_back(data[i]);

    }
}

vector<rt_uint8_t> MemoryIOStream::getBuffer() {
    auto result = vector<rt_uint8_t>{};
    copy(buf.begin(), buf.end(), back_inserter(result));
    return result;
}
