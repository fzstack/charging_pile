/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-04-01     imgcr       the first version
 */


#ifdef TEST_USER
#include <rtthread.h>
#include <applications/user.hxx>

int init_test_user() {
    Preset::User::get();
    rt_kprintf("user init ok\n");
    return RT_EOK;
}

INIT_APP_EXPORT(init_test_user);

#endif

