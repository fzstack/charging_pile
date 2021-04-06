/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-04-06     imgcr       the first version
 */
#pragma once
#include <rtthread.h>
#include <utilities/math.hxx>

//不保证线程安全
template<class T>
class Vel {
public:
    Vel(T initVal, float posStep): posStep(posStep), originVal(initVal), currVal(initVal), targetVal(initVal) {

    }

    void setTarget(T t) {
        originVal = currVal;
        targetVal = t;
        currPos = 0;
    }

    void update() {
        if(is_same_val(currPos, 1.f)) {
            return;
        }
        currPos = std::min(currPos + posStep, 1.f);
        currVal = intepl(originVal, targetVal, ease(currPos));
    }

    T get() {
        return currVal;
    }

    virtual float ease(float x) {
        return x;
    }

protected:
    virtual T intepl(const T& originVal, const T& targetVal, float pos) = 0;

private:
    const float posStep;
    float currPos = 0;
    T originVal, currVal, targetVal;
};

static inline float vel_step(int durationMs, int tickFreqHz) {
    return 1000.f / durationMs / tickFreqHz;
}


