/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-12-19     imgcr       the first version
 */

#include "ali_iot_device.hxx"

#define LOG_TAG "test.ali_iot"
#define LOG_LVL LOG_LVL_DBG
#include <ulog.h>

using namespace std;
using namespace json_literals;

#ifdef TEST_ALI_IOT_DEVICE
static void test_ali_iot_device() {

    auto air724 = Instances::air724;
    air724->init();

    auto aliIot = Instances::aliIotDevice;
    try {

        aliIot->services["query"] += [](const Json params) -> Json {
            rt_kprintf("\033[34mparams: %s\n\033[0m", to_string(params).c_str());
            return {};
        };

        aliIot->services["control"] += [](const Json params) -> Json {
            rt_kprintf("\033[34mport: %d\n\033[0m", params["not_found"_i]);
            rt_kprintf("\033[34mtimer_id: %d\n\033[0m", params["timer_id"_i]);
            rt_kprintf("\033[34mminutes: %d\n\033[0m", params["minutes"_i]);
            return {
                {"state", 1},
            };
        };

        aliIot->login("863701042917152", "a1tltf2GJUn", "e96d5f79301c20994cb2e984e3cad47b");

//
//        aliIot->servcies["control"] += [](auto data) {
//
//        };

//        aliIot->on("control", [](auto data) {
//
//           return Json{};
//        });
    } catch(exception& e) {
        LOG_E("%s", e.what());
    }
}

static int init_test_ali_iot_device() {
//    auto air724 = Instances::air724;
//    try {
//        air724->init();
//        LOG_D("inited");
//    } catch(exception& e) {
//        LOG_E("init failed, %s", e.what());
//    }
    return RT_EOK;
}

MSH_CMD_EXPORT(test_ali_iot_device, );
INIT_APP_EXPORT(init_test_ali_iot_device);
#endif
