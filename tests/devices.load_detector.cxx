/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-12-14     imgcr       the first version
 */
#ifdef TEST_LOAD_DETECTOR

#include <devices/load_detector.hxx>
#include <utilities/cmd.hxx>
#include <rtthread.h>
#include <functional>

#define LOG_TAG "test.load_det"
#define LOG_LVL LOG_LVL_DBG
#include <ulog.h>

using namespace std;
using namespace placeholders;

static int init_test_load_detector() {
    auto lodDetA = Preset::LoadDetector<0>::get();
    auto lodDetB = Preset::LoadDetector<1>::get();
    lodDetA->oState += [](auto state){
        if(state) {
            LOG_D("A %s", *state ? "pluged" : "unpluged");
        }
    };
    lodDetB->oState += [](auto state){
        if(state) {
            LOG_D("B %s", *state ? "pluged" : "unpluged");
        }
    };
    LOG_I("inited, plug to test");
    return RT_EOK;
}

INIT_APP_EXPORT(init_test_load_detector);
#endif
