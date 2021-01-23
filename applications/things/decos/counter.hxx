/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-01-03     imgcr       the first version
 */
#pragma once

#include "base.hxx"
#include <utilities/observable.hxx>

namespace Things::Decos {
/**
 * @description: 倒计时功能类
 */
class Counter: public Base {
    friend outer_t;
    Counter(outer_t* outer);
    virtual void init() override;

    Observable<bool> inited = false;

    Timer timer = {1000, kTimer};
    static const char* kTimer;
};
}
