/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-04-08     imgcr       the first version
 */
#pragma once
#include <string_view>

class Rebooter {
public:
    virtual void reboot() = 0;
    virtual void rebootAll() {};
    virtual void rebootModule(std::string_view module) {};
};


