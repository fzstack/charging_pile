/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-12-28     imgcr       the first version
 */
#ifndef CONFIG_APP_HXX_
#define CONFIG_APP_HXX_
#include <string>

namespace Config {
class App {
public:
    static const char
        *cloudDeviceName,
        *cloudProductKey,
        *cloudDeviceSecret;
};
}

enum class State {
    LoadNotInsert = 1,
    LoadInserted,
    Charging,
    LoadWaitRemove,
    Error,
};

std::string getStateStr(State state);


#endif /* CONFIG_APP_HXX_ */
