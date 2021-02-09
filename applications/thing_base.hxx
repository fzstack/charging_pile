/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-02-05     imgcr       the first version
 */
#pragma once
#include <utilities/signals.hxx>
#include <string>
#include <array>
#include <config/app.hxx>
#include <config/bsp.hxx>

class ThingBase {
public:
    virtual void control(int port, int timerId, int minutes) = 0;
    virtual void stop(int port, int timerId) = 0;
    virtual void config(int currentLimit, int uploadThr, int fuzedThr, int noloadCurrThr) = 0;
    virtual std::array<CurrentData, Config::Bsp::kPortNum> getCurrentData() = 0;

    Signals<void(int port)> onPortAccess;
    Signals<void(int port)> onPortUnplug;
    Signals<void(int port, std::string icNumber)> onIcNumber;
    Signals<void(int port)> onCurrentLimit;
    Signals<void()> onCurrentData;
};


