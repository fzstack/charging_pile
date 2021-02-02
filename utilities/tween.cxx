/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-02-01     imgcr       the first version
 */

#include "tween.hxx"

using namespace rtthread;

float TweenBase::ease(float x) {
    return x;
}

Mutex TweenBase::mutex = {kMutex};
