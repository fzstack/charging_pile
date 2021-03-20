/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-03-20     imgcr       the first version
 */
#pragma once

#include "istream.hxx"
#include "ostream.hxx"
#include <deque>
#include <vector>
#include <rtthread.h>

class MemoryIOStream: public IStream, public OStream {
public:
    virtual void readData(rt_uint8_t* data, int len) override final;
    virtual void writeData(rt_uint8_t* data, int len) override final;
    std::vector<rt_uint8_t> getBuffer();
private:
    std::deque<rt_uint8_t> buf;

};
