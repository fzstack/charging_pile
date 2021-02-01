/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-01-31     imgcr       the first version
 */


#ifdef TEST_STATIC_CTOR
#include <utilities/static_ctor.hxx>
#include <rtthread.h>
#include <memory>

using namespace std;

class TestStaticCtor: public StaticCtor<TestStaticCtor> {
    friend static_ctor_t;
public:
    TestStaticCtor() {
        rt_kprintf("ctor called\n");
    }

    void test() {
        rt_kprintf("test func called\n");
    }
private:
    static void staticCtor() {
        rt_kprintf("static ctor called\n");
    }
};

static void test_static_ctor() {
    auto test = make_shared<TestStaticCtor>();
    test->test();
}

MSH_CMD_EXPORT(test_static_ctor, )
#endif
