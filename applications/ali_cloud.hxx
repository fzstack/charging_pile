/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-12-30     imgcr       the first version
 */
#ifndef APPLICATIONS_ALI_CLOUD_HXX_
#define APPLICATIONS_ALI_CLOUD_HXX_

#include "cloud.hxx"
#include "ali_iot_device.hxx"
#include <devices/air724.hxx>
#include <memory>

class AliCloud: public Cloud {
public:
    AliCloud(std::shared_ptr<AliIotDevice> device, std::shared_ptr<Air724> air, std::shared_ptr<CloudTimer> timer);
    virtual void init() override;
    virtual void setCurrentData(std::array<CurrentData, Config::Bsp::kPortNum>& data) override;

    //下面两个函数是云类内部调用的
    virtual void setIccid(std::string_view iccid) override;
    virtual void setSignal(int signal) override;

    virtual void emitPortAccess(int port) override;
    virtual void emitPortUnplug(int port) override;
    virtual void emitIcNumber(int port, std::string_view icNumber) override;
    virtual void emitCurrentLimit(int port) override;

    template<class... Args>
    auto post(Args&&... args) {
        return device->thread->post(std::forward<Args>(args)...);
    }

protected:
    virtual void setSignalInterval() override;

private:
    std::shared_ptr<AliIotDevice> device;
    std::shared_ptr<Air724> air;
    Signals<void()> signal = {};
    Observable<bool> inited = {false};
};

#include <utilities/singleton.hxx>
namespace Preset {
class AliCloud: public Singleton<AliCloud>, public ::AliCloud {
    friend class Singleton<AliCloud>;
    AliCloud(): ::AliCloud(AliIotDevice::get(), Air724::get(), CloudTimer::get(this)) {};
};
}

#endif /* APPLICATIONS_ALI_CLOUD_HXX_ */
