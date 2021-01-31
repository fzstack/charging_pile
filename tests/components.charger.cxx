/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-12-31     imgcr       the first version
 */
#ifdef TEST_CHARGER
#include <components/charger.hxx>
#include <utilities/cmd.hxx>
#include <map>
#include <config/bsp.hxx>

#define LOG_TAG "test.charger"
#define LOG_LVL LOG_LVL_DBG
#include <ulog.h>

using namespace std;

std::shared_ptr<Charger> getCharger(int i) {
    switch(i) {
    case 0:
        return Preset::Charger<0>::get();
    case 1:
        return Preset::Charger<1>::get();
    default:
        throw out_of_range{"resource id out of range"};
    }
}

static void test_charger(int argc, char** argv) {
    ASSERT_MIN_NARGS(3);

    auto i = atoi(argv[1]);
    ASSERT_ARG(charger, 0 <= i && i < Config::Bsp::kPortNum);

    auto cmds = map<string, function<void()>> {
        {"start", [=](){
            getCharger(i)->start();
        }},
        {"stop", [=](){
            getCharger(i)->stop();
        }},
    };
    auto found = cmds.find(argv[2]);
    ASSERT_ARG(cmd, found != cmds.end());
    found->second();
}

static int init_test_charger() {
    for(auto i = 0; i < Config::Bsp::kPortNum; i++) {
        getCharger(i)->init();
    }
    return RT_EOK;
}


MSH_CMD_EXPORT(test_charger, );
INIT_APP_EXPORT(init_test_charger);
#endif

