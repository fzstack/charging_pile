/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-01-28     imgcr       the first version
 */

#ifdef TEST_TTP229
#include <devices/ttp229.hxx>
#include <rtthread.h>

int init_test_ttp229() {
    auto ttp229 = Preset::Ttp229::get();
    ttp229->onData += [](auto value){
        rt_kprintf("data: %d\n", value);
    };
    rt_kprintf("ttp229 test inited\n");
    return RT_EOK;
}

void test_x() {
    rt_kprintf("x\n");
}

INIT_APP_EXPORT(init_test_ttp229);
MSH_CMD_EXPORT(test_x, );

#endif
