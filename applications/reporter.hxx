/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-04-11     imgcr       the first version
 */
#pragma once

#include <components/keyboard.hxx>
#include <devices/rc522.hxx>
#include <applications/ali_iot_device.hxx>
#include <components/app_state.hxx>

class Reporter {
public:
    Reporter(std::shared_ptr<Rc522Base> rc522, std::shared_ptr<Keyboard> keyboard, std::shared_ptr<AliIotDevice> device, std::shared_ptr<AppState> appState);
};


