/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-01-20     imgcr       the first version
 */
#pragma once

#include "base.hxx"
#include <utilities/observable.hxx>
#include <config/bsp.hxx>
#include <components/timer.hxx>

namespace Things::Decos {
/**
 * @description: 状态备份功能类
 */
class Backuper: public Base {
    friend outer_t;
    Backuper(outer_t* outer);
    virtual void init() override;

    Observable<bool> inited = false;

public:
    template<int N>
    struct Backup {
        int leftSeconds = 0;
        int timerId = 0;
        float consumption = 0;
    };

private:
    struct PortSpec {
        std::shared_ptr<Timer> timer;
        bool stateHasTransitioned = false;
    };

    std::array<PortSpec, Config::Bsp::kPortNum> specs;
    static constexpr int kDuration = 10000;
    static const char* kTimer;

};
}

