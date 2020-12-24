/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-08-26     imgcr       the first version
 */
#ifndef UTILITIES_LOCK_GUARD_HXX_
#define UTILITIES_LOCK_GUARD_HXX_

#include <rtthread.h>

//TODO: DEPRESSED
class LockGuard {
public:
    LockGuard(rt_mutex_t mutex): mutex(mutex) {
        rt_mutex_take(mutex, RT_WAITING_FOREVER);
    }

    ~LockGuard() {
        rt_mutex_release(mutex);
    }

private:
    rt_mutex_t mutex;
};



#endif /* UTILITIES_LOCK_GUARD_HXX_ */
