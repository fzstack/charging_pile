/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-04-12     imgcr       the first version
 */
#pragma once

#include <utilities/signals.hxx>

template<class T>
struct Rewire;

template<class R, class ...P>
struct Rewire<R(P...)> {
public:
    Rewire(Signals<R(P...)>& signal): signal(signal) {

    }

    template<class T>
    void operator+=(T cb) {
        signal += cb;
    }

private:
    Signals<R(P...)>& signal;
};
