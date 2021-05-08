/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-04-11     imgcr       the first version
 */

#include "reporter.hxx"
#include <string>
using namespace std;
using namespace string_literals;

Reporter::Reporter(std::shared_ptr<Rc522Base> rc522, std::shared_ptr<Keyboard> keyboard, std::shared_ptr<AliIotDevice> device, std::shared_ptr<AppState> appState) {
//    rc522->oCardId += [=](auto value) {
//        if(!value) return;
//        if(!appState->cloudConnected) return;
//        device->log("card: "s + *value);
//    };
//    keyboard->oValue += [=](auto value) {
//
//    };
}
