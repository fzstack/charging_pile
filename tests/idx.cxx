/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-01-13     imgcr       the first version
 */

#include <array>
#include <rtthread.h>
#include <utilities/idx.hxx>

#ifdef TEST_IDX

using namespace std;

class TestOwner {
public:
    void read(rt_uint8_t addr, rt_uint8_t* data, std::size_t size) {
        rt_kprintf("read %02x, size %d: ", addr, size);
        for(auto i = 0u; i < size; i++) {
            rt_kprintf("%02x ", data[i]);
        }
        rt_kprintf("\n");
        memcpy(data, &buf[addr], size);
    }

    void write(rt_uint8_t addr, rt_uint8_t* data, std::size_t size) {
        rt_kprintf("write %02x, size %d: ", addr, size);
        for(auto i = 0u; i < size; i++) {
            rt_kprintf("%02x ", data[i]);
        }
        rt_kprintf("\n");
        memcpy(&buf[addr], data, size);
    }

    static std::shared_ptr<TestOwner> get() {
        return *owner;
    }

    static ThreadLocal<std::shared_ptr<TestOwner>> owner;

private:
    std::array<rt_uint8_t, 0xf0> buf;
};

ThreadLocal<std::shared_ptr<TestOwner>> TestOwner::owner = {nullptr};

template<class T>
using TestIdx = Idx<T, TestOwner, rt_uint8_t, 0xff, 0x1f>;

struct TestStruct {
    int fieldA = 0;
    int fieldB = 0;
};

static void test_idx() {

    try {
        auto testOwner = make_shared<TestOwner>();
        testOwner->owner = testOwner;

        rt_kprintf("size of idx: %d\n", sizeof(TestIdx<TestStruct>));

        auto test1 = TestIdx<TestStruct>(0);
        auto test2 = TestIdx<TestStruct>(8);
        auto test3 = TestIdx<TestStruct>(8);
        auto test4 = TestIdx<TestStruct>(24);
        auto test5 = TestIdx<TestStruct>(16);

        test1->fieldA = 5;
        test2->fieldA = 233;
        test3->fieldB = 233;
        test4->fieldA = 5;
        test5->fieldA = 6;


    } catch(const exception& e) {
        rt_kprintf("err: %s\n", e.what());
    }

}

MSH_CMD_EXPORT(test_idx, );

#endif
