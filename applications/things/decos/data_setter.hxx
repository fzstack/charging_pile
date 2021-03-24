/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-01-21     imgcr       the first version
 */
#pragma once

#include "base.hxx"
#include <utilities/observable.hxx>
#include <array>
#include <config/bsp.hxx>

namespace Things::Decos {
/**
 * @description: 状态数据上报功能类
 */
class DataSetter: public Base {
    friend outer_t;
    DataSetter(outer_t* outer);
    virtual void init() override;
    virtual void config(int currentLimit, int uploadThr, int fuzedThr, int noloadCurrThr) override;



public:
    struct Params {
        int currDiffThrMiA = 10; //电流变化阈值
    };

private:
    struct ChargerSpec {
        int prevCurrMiA = 0;
        rt_tick_t prevTick = 0;
    };

private:
    std::array<ChargerSpec, Config::Bsp::kPortNum> specs;
    Observable<bool> inited = false;
};
}

