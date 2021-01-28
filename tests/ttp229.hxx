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

int init_ttp229() {
    auto ttp229 = Preset::Ttp229::get();
    ttp229->onData += [](auto value){
        rt_kprintf("data: %04x", value);
    };
}

MSH_CMD_EXPORT(init_ttp229, );

#endif
