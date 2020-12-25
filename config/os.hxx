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

namespace Config {
class Os {
public:
    static constexpr int
        kThreadDefaultStackSize = 2048,
        kAliIotDeviceThreadPrio = 10,
        kAliIotDeviceThreadStackSize = kThreadDefaultStackSize;

    static const char
        *kAliIotDeviceThreadName;
};
}



#endif /* CONFIG_OS_HXX_ */
