/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-12-14     imgcr       the first version
 */

#include <applications/instances.hxx>
#include <utilities/cmd.hxx>
#include <rtthread.h>
#include <tests/air724.hxx>
#include <exception>

#define LOG_TAG "test.air724"
#define LOG_LVL LOG_LVL_DBG
#include <ulog.h>

using namespace std;

#ifdef TEST_AIR724
static void test_air724() {
    auto air724 = Instances::air724;
    try {
        auto csq = air724->getCsq();
        auto imei = air724->getImei();
        auto iccid = air724->getIccid();

        LOG_I("CSQ: %d", csq);
        LOG_I("imei: %s", imei.c_str());
        LOG_I("iccid: %s", iccid.c_str());
    } catch(exception& e) {
        LOG_E("%s", e.what());
    }
}

static void test_air724_reset() {
    auto air724 = Instances::air724;
    air724->reset();
}

static int init_test_air724() {
    auto air724 = Instances::air724;
    try {
        air724->init();
        LOG_D("inited");
    } catch(exception& e) {
        LOG_E("init failed, %s", e.what());
    }
    return RT_EOK;
}

MSH_CMD_EXPORT(test_air724, );
MSH_CMD_EXPORT(test_air724_reset, );
INIT_APP_EXPORT(init_test_air724);
#endif


