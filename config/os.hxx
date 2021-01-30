/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-12-26     imgcr       the first version
 */
#ifndef CONFIG_OS_HXX_
#define CONFIG_OS_HXX_

#include <rtthread.h>

namespace Config {
class Os {
public:
    static constexpr int
        kThreadDefaultStackSize = 4096,
        kAliIotDeviceThreadPrio = 10,
        kAliIotDeviceThreadStackSize = kThreadDefaultStackSize;

    static const char
        *kAliIotDeviceThreadName;
};
}

enum class Priority {
    High,
};

template<Priority Proi>
struct PriorityTrait {
    //static constexpr rt_uint8_t value = 0;
};

template<>
struct PriorityTrait<Priority::High> {
    static constexpr rt_uint8_t value = 2;
};





#endif /* CONFIG_OS_HXX_ */
