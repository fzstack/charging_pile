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
 * @description: 事件上报功能类
 */
class EventEmitter: public Base {
    friend outer_t;
    EventEmitter(outer_t* outer);
    virtual void init() override;

private:
    Observable<bool> inited = false;
};
}
