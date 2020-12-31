/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-12-31     imgcr       the first version
 */

#include "virtual_load_detector.hxx"
#include <components/multimeter.hxx>
#include <rtthread.h>

#define LOG_TAG "test.vld"
#define LOG_LVL LOG_LVL_DBG
#include <ulog.h>

#ifdef TEST_VIRTUAL_LOAD_DETECTOR
void test_virual_load_detector_init() {
    auto mulmtr = Preset::Multimeter::get();
    auto vlodet = Preset::VirtualLoadDetector<0>::get();

    vlodet->oState += [](auto value) {
        if(value) {
            if(*value) {
                LOG_I("on");
            } else {
                LOG_I("off");
            }
        } else {
            LOG_I("unknown");
        }
    };

    mulmtr->init();
    vlodet->init();
}


MSH_CMD_EXPORT(test_virual_load_detector_init, );
#endif
