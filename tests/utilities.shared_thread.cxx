/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-01-29     imgcr       the first version
 */

#ifdef TEST_SHARED_THREAD

#include <utilities/shared_thread.hxx>
#include <rtthread.h>

void test_shared_thread() {
    auto high = Preset::SharedThread<Priority::High>::get();
    high->exec([](){
       rt_kprintf("test self is: %s\n", rt_thread_self()->name);
    });
}

MSH_CMD_EXPORT(test_shared_thread, );

#endif

