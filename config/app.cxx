/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-12-28     imgcr       the first version
 */

#include "app.hxx"
#include <map>
#include <utilities/err.hxx>

using namespace Config;
using namespace std;

const char
    *App::cloudDeviceName = "863701042917152",
    *App::cloudProductKey = "a1tltf2GJUn",
    *App::cloudDeviceSecret = "e96d5f79301c20994cb2e984e3cad47b";

string getStateStr(State::Value state) {
    switch(state) {
    case State::LoadNotInsert:
        return "load_not_insert";
    case State::LoadInserted:
        return "load_inserted";
    case State::Charging:
        return "charging";
    case State::LoadWaitRemove:
        return "load_wait_remove";
    case State::Error:
        return "error";
    default:
        throw not_implemented{"unknown_state"};
    }
}
