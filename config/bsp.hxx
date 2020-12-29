/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-12-29     imgcr       the first version
 */
#ifndef CONFIG_BSP_HXX_
#define CONFIG_BSP_HXX_
#include <type_traits>

namespace Config {
class Bsp {
public:
    static constexpr int kPortNum = 2;

    template<class T, int R>
    using assert_t = std::enable_if_t<R >= 0 && R < kPortNum, T>;
};
}



#endif /* CONFIG_BSP_HXX_ */
