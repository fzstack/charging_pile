/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-12-31     imgcr       the first version
 */

#include "charger.hxx"
#include <utilities/cmd.hxx>
#include <map>

#define LOG_TAG "test.charger"
#define LOG_LVL LOG_LVL_DBG
#include <ulog.h>

using namespace std;

#ifdef TEST_CHARGER

static void test_charger(int argc, char** argv) {
    ASSERT_MIN_NARGS(2);
    auto cmds = map<string, function<void()>> {
        {"start", [](){
            Preset::Charger<0>::get()->start();
        }},
        {"stop", [](){
            Preset::Charger<0>::get()->stop();
        }},
    };
    auto found = cmds.find(argv[1]);
    ASSERT_ARG(cmd, found != cmds.end());
    found->second();
}

static int init_test_charger() {
    Preset::Multimeter::get()->init();
    Preset::VirtualLoadDetector<0>::get()->init();
    Preset::Charger<0>::get()->init();
    return RT_EOK;
}


MSH_CMD_EXPORT(test_charger, );
INIT_APP_EXPORT(init_test_charger);
#endif

