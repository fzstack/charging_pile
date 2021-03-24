/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-01-19     imgcr       the first version
 */
#pragma once

#include "base.hxx"
#include <utilities/observable.hxx>
#include <array>
#include <components/timer.hxx>
#include <config/bsp.hxx>
#include <Mutex.h>

namespace Things::Decos {
/**
 * @description: 限流功能类
 */
class CurrentLimiter: public Base {
    friend outer_t;
    CurrentLimiter(outer_t* outer);
    virtual void init() override;
    virtual void config(int currentLimit, int uploadThr, int fuzedThr, int noloadCurrThr) override;

public:
    struct Params {
        int maxCurrentMiA = 150; //电流变化阈值
    };

private:
    Observable<bool> inited = false;
    std::array<std::shared_ptr<Timer>, Config::Bsp::kPortNum> timers;

    rtthread::Mutex mutex;

private:
    static constexpr int kDuration = 2000;
    static const char* kTimer;
    static const char* kMutex;
};
}

