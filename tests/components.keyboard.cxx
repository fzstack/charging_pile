/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-02-04     imgcr       the first version
 */

#ifdef TEST_KEYBOARD
#include <rtthread.h>
#include <components/keyboard.hxx>
#include <map>

#define LOG_TAG "test.kboard"
#define LOG_LVL LOG_LVL_DBG
#include <ulog.h>

using namespace std;

int init_test_keyboard() {
    auto keyboard = Preset::Keyboard::get();
    keyboard->oValue += [](auto value){
        if(!value) return;
        auto s = map<Keyboard::Keys, string>{
            {Keyboard::Keys::K0, "k0"},
            {Keyboard::Keys::K1, "k1"},
            {Keyboard::Keys::K2, "k2"},
            {Keyboard::Keys::K3, "k3"},
            {Keyboard::Keys::K4, "k4"},
            {Keyboard::Keys::K5, "k5"},
            {Keyboard::Keys::K6, "k6"},
            {Keyboard::Keys::K7, "k7"},
            {Keyboard::Keys::K8, "k8"},
            {Keyboard::Keys::K9, "k9"},
            {Keyboard::Keys::Ret, "ret"},
            {Keyboard::Keys::Ok, "ok"},
        };
        auto found = s.find(*value);
        if(found == s.end()) return;
        LOG_I("%s clicked!", found->second.c_str());
    };
    return RT_EOK;
}

INIT_APP_EXPORT(init_test_keyboard);

#endif
