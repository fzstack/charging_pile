/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-01-31     imgcr       the first version
 */
#pragma once

#include <limits>
#include <algorithm>

template <class T>
float normalize(T t) {
    return (float(t) - std::numeric_limits<T>::min()) / (float(std::numeric_limits<T>::max()) - std::numeric_limits<T>::min());
}

template<typename T>
T saturate(T val, T min, T max) {
    return std::min(std::max(val, min), max);
}
