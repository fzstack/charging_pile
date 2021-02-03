/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-02-03     imgcr       the first version
 */
#pragma once

#include "app.hxx"

class App;
class LowerApp: public App {
public:
    virtual void run() override;
};


