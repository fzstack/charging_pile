/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-04-01     imgcr       the first version
 */

#include <utilities/tiny_type_id.hxx>
#include <rtthread.h>

#ifdef TEST_TINY_TYPE_ID

template<class T>
struct Wrapper{

};

void test_tiny_type_id() {
    rt_kprintf("hash of int: %08x\n", TypeId<int>::get());
    rt_kprintf("hash of long: %08x\n", TypeId<long>::get());
    rt_kprintf("hash of Wrapper<int>: %08x\n", TypeId<Wrapper<int>>::get());
    rt_kprintf("hash of Wrapper<long>: %08x\n", TypeId<Wrapper<long>>::get());
}

MSH_CMD_EXPORT(test_tiny_type_id, );

#endif
