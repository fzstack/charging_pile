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
#include <utilities/mp.hxx>

#define LOG_TAG "test.charger"
#define LOG_LVL LOG_LVL_DBG
#include <ulog.h>

using namespace std;

static void test_charger(int argc, char** argv) {
    ASSERT_MIN_NARGS(3);

    auto r = atoi(argv[1]);
    ASSERT_ARG(charger, 0 <= r && r < Config::Bsp::kPortNum);

    auto charger = shared_ptr<Charger>{};

    magic_switch<Config::Bsp::kPortNum>{}([&](auto v){
        charger = Preset::Charger<decltype(v)::value>::get();
    }, r);

    auto cmds = map<string, function<void()>> {
        {"start", [&](){
            charger->start();
        }},
        {"stop", [&](){
            charger->stop();
        }},
    };
    auto found = cmds.find(argv[2]);
    ASSERT_ARG(cmd, found != cmds.end());
    found->second();
}

static int init_test_charger() {
    for(auto i = 0; i < Config::Bsp::kPortNum; i++) {
        magic_switch<Config::Bsp::kPortNum>{}([&](auto v){
            rt_kprintf("initing %d\n", i);
            Preset::Charger<decltype(v)::value>::get()->init();
            rt_uint32_t used;
            rt_memory_info(nullptr, &used, nullptr);
            printf("used: %d\n", used);
        }, i);
    }
    return RT_EOK;
}


MSH_CMD_EXPORT(test_charger, );
INIT_APP_EXPORT(init_test_charger);
#endif

