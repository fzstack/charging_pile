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

struct State {
    enum Value {
        LoadNotInsert = 1,
        LoadInserted,
        Charging,
        LoadWaitRemove,
        Error,
    };
};

struct CurrentData {
    int port;
    int timerId;
    int leftMinutes;
    State::Value state;
    float current;
    float voltage;
    float consumption;
    struct Fuse {
        enum Value {
            Normal,
            Fused,
            Unknown,
        };
    };
    Fuse::Value fuse;
};

std::string getStateStr(State::Value state);


#endif /* CONFIG_APP_HXX_ */
