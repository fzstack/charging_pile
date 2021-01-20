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

#include <memory>
#include <string_view>
#include <array>
#include <config/bsp.hxx>
#include "cloud_timer.hxx"
#include <utilities/signals.hxx>
#include <config/app.hxx>

class CloudTimer;

class Cloud {
    friend class CloudTimer;
public:
    Cloud(std::shared_ptr<CloudTimer> timer);
    virtual void init();

    virtual void setCurrentData(std::array<CurrentData, Config::Bsp::kPortNum>& data) = 0;
    virtual void setIccid(std::string_view iccid) = 0;
    virtual void setSignal(int signal) = 0;

    virtual void emitPortAccess(int port) = 0;
    virtual void emitIcNumber(int port, std::string_view icNumber) = 0;
    virtual void emitCurrentLimit(int port) = 0;

    Signals<void()> onQuery = {};

    struct ServiceResult {
        enum Value {
            Succeed = 1, Failed,
        };
    };
    Signals<ServiceResult::Value(int port, int timerId, int minutes)> onControl = {};
    Signals<ServiceResult::Value(int port, int timerId)> onStop = {};

protected:
    virtual void setSignalInterval() = 0;

private:
    std::shared_ptr<CloudTimer> timer;
};


#endif /* APPLICATIONS_CLOUD_HXX_ */
