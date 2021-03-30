/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-03-29     imgcr       the first version
 */
#pragma once
#include <rtthread.h>

template<class T = rt_uint8_t>
class CountDown {
public:
    CountDown(T initVal = 1): currVal(initVal), initVal(initVal) {

    }
    void reset() {
        counting = false;
        currVal = initVal;
    }

    void trigger() {
        counting = true;
    }

    void forceTrigger() {
        counting = true;
        currVal = 1;
    }

    void retrigger() {
        counting = true;
        if(currVal == 0) {
            currVal = initVal;
        }
    }

    bool updateAndCheck() {
        if(!counting) return false;
        if(currVal == 0) return false;
        if(--currVal == 0) return true;
        return false;
    }

private:
    bool counting = false;
    T currVal;
    const T initVal;
};


