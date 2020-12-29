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
#include <array>
#include <config/bsp.hxx>
#include "cloud_timer.hxx"

class CloudTimer;

class Cloud {
    friend class CloudTimer;
public:
    Cloud(std::shared_ptr<AliIotDevice> device, std::shared_ptr<Air724> air, std::shared_ptr<CloudTimer> timer);
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
    void setCurrentData(std::array<CurrentData, Config::Bsp::kPortNum> data);

private:
    void setIccid(std::string_view iccid);
    void setSignal(int signal); //TODO: 自动发送信号强度?

    std::shared_ptr<AliIotDevice> device;
    std::shared_ptr<Air724> air;
    std::shared_ptr<CloudTimer> timer;
};

#include <utilities/singleton.hxx>
namespace Preset {
class Cloud: public Singleton<Cloud>, public ::Cloud {
    friend class Singleton<Cloud>;
    Cloud();
};
}

#endif /* APPLICATIONS_CLOUD_HXX_ */
