/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-12-14     imgcr       the first version
 */

#include "wtn6.hxx"

#define LOG_TAG "test.wtn6"
#define LOG_LVL LOG_LVL_DBG
#include <ulog.h>

void test_wtn6(int argc, char** argv) {
    ASSERT_MIN_NARGS(2);

    auto wtn6 = Instances::wtn6;
    auto data = atoi(argv[1]);
    ASSERT_ARG(voice_id, 0 <= data && data <= 15);
    wtn6->write(data);
}

#ifdef TEST_WTN6
MSH_CMD_EXPORT(test_wtn6,);
#endif


