/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-12-30     imgcr       the first version
 */

#ifdef TEST_ALI_CLOUD
#include <rtthread.h>
#include <applications/ali_cloud.hxx>
#include <utilities/cmd.hxx>

#define LOG_TAG "test.a_cloud"
#define LOG_LVL LOG_LVL_DBG
#include <ulog.h>

using namespace std;

static void test_ali_cloud_init() {
    auto cloud = Preset::AliCloud::get();
    cloud->onQuery += []() {
        rt_kprintf("\033[95mcloud on query\n\033[0m");
    };

    cloud->onControl += [](int port, int timerId, int minutes) {
        rt_kprintf("\033[95mport: %d\ntimer_id: %d\nminutes: %d\n\033[0m", port, timerId, minutes);
        return Cloud::ServiceResult::Succeed;
    };

    cloud->onStop += [](int port, int timerId) {
        rt_kprintf("\033[95mport: %d\ntimer_id: %d\n\033[0m", port, timerId);
        return Cloud::ServiceResult::Succeed;
    };

    cloud->init();
}

//static void test_ali_cloud_cur_data() {
//    auto cloud = Preset::AliCloud::get();
//    cloud->setCurrentData({});
//}

static void test_ali_cloud_emit_port_access(int argc, char** argv) {
    ASSERT_MIN_NARGS(2);
    auto cloud = Preset::AliCloud::get();
    auto port = atoi(argv[1]);
    cloud->emitPortAccess(port);
}

MSH_CMD_EXPORT(test_ali_cloud_init, );
//MSH_CMD_EXPORT(test_ali_cloud_cur_data, );
MSH_CMD_EXPORT(test_ali_cloud_emit_port_access, );
#endif
