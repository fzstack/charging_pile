/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-12-13     imgcr       the first version
 */
#ifdef TEST_RC522
#include <devices/rc522_preset.hxx>
#include <rtthread.h>

#define LOG_TAG "test.rc522"
#define LOG_LVL LOG_LVL_DBG
#include <ulog.h>

static void test_rc522() {

}

static int init_test_rc522() {
    auto rc522 = Preset::Rc522::get();
    rc522->oCardId += [](auto value) {
        if(value) {
            LOG_D("card is: %s", value->c_str());
        } else {
            LOG_D("card removed");
        }
    };

    rc522->init();
    LOG_D("rc522 init ok.");
    LOG_I("swipe card to test");
    return RT_EOK;
}


MSH_CMD_EXPORT(test_rc522, );
INIT_APP_EXPORT(init_test_rc522);
#endif
