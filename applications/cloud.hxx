/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-12-28     imgcr       the first version
 */
#ifndef APPLICATIONS_CLOUD_HXX_
#define APPLICATIONS_CLOUD_HXX_

#include "ali_iot_device.hxx"
#include <devices/air724.hxx>
#include <memory>
#include <string_view>

class Cloud {
public:
    Cloud(std::shared_ptr<AliIotDevice> device, std::shared_ptr<Air724> air);
    void init();

    struct CurrentData {
        int port;
        int timerId;
        int leftMinutes;
        struct State {
            enum Value {
                LoadNotReady = 1,
                LoadReady,
                Charging,
                Charged,
                Error,
            };
        };
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
    void setCurrentData();

private:
    void setIccid(std::string_view iccid);
    void setSignal(int signal); //TODO: 自动发送信号强度?

    std::shared_ptr<AliIotDevice> device;
    std::shared_ptr<Air724> air;

};



#endif /* APPLICATIONS_CLOUD_HXX_ */
