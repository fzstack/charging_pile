/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-01-02     imgcr       the first version
 */

#ifdef TEST_THING
#include <applications/thing_preset.hxx>
#include <map>
#include <string>
#include <functional>
#include <utilities/cmd.hxx>

#define LOG_TAG "test.thing"
#define LOG_LVL LOG_LVL_DBG
#include <ulog.h>

using namespace std;

//test_thing control [port] [timer_id] [minutes]
void test_thing(int argc, char** argv) {
    ASSERT_MIN_NARGS(4);
    auto port = atoi(argv[2]);
    ASSERT_ARG(port, 0 <= port && port < Config::Bsp::kPortNum);
    auto timerId = atoi(argv[3]);
    auto thing = Preset::Thing::get();
    auto select = map<string, function<void()>> {
        {"control", [=](){
            ASSERT_MIN_NARGS(5);
            auto minutes = atoi(argv[4]);
            ASSERT_ARG(minutes, minutes > 0);
            thing->control(port, timerId, minutes);
        }},
        {"stop", [=](){
            thing->stop(port, timerId);
        }},
    };
    auto found = select.find(argv[1]);
    ASSERT_ARG(cmd, found != select.end());

    try {
        found->second();
    } catch(const exception& e) {
        LOG_E("{%s} %s", typeid(e).name(), e.what());
    }
}

int init_test_thing() {

    auto thing = Preset::Thing::get();

    thing->onPortAccess += [](auto port) {
        LOG_I("port accessed: %d", port);
    };

    thing->onIcNumber += [](auto port, auto icNumber) {
        LOG_I("ic number: %d, %s", port, icNumber.c_str());
    };

    return RT_EOK;
}

MSH_CMD_EXPORT(test_thing, );
INIT_APP_EXPORT(init_test_thing);
#endif
