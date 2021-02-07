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
//867435054397377
const char
    *App::cloudDeviceName = "862167057563607",
    *App::cloudProductKey = "a1mKT2XMEnq",
    *App::cloudDeviceSecret = "df6581d7fc78c58bdb06cbd7f3c1afd0";

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
