/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-12-14     imgcr       the first version
 */

#include "hlw8112.hxx"

#define LOG_TAG "test.hlw8112"
#define LOG_LVL LOG_LVL_DBG
#include <ulog.h>

using namespace Hlw8112ns::Regs;

#ifdef TEST_HLW8112
static void test_hlw8112() {
    LOG_I("if init Ok, then test passed");
}

static int init_test_hlw8112() {
    auto hlw8112 = Preset::Hlw8112::get();

    LOG_I("initing");
    hlw8112->init();
    LOG_I("inited");

    return RT_EOK;
}

MSH_CMD_EXPORT(test_hlw8112, );
INIT_APP_EXPORT(init_test_hlw8112);
#endif
