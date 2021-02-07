/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-02-02     imgcr       the first version
 */

#ifdef TEST_STL
#include <vector>
#include <array>
#include <list>
#include <forward_list>
#include <set>
#include <functional>
#include <utilities/signals.hxx>
#include <optional>
#include <utilities/observable.hxx>
#include <utilities/observer.hxx>

#define LOG_TAG "test.stl"
#define LOG_LVL LOG_LVL_DBG
#include <ulog.h>

using namespace std;

//list: 12字节
//list节点: 20字节+sizeof(T)
//function: 16字节

void test_stl_vector() {
    rt_uint32_t used;
    rt_memory_info(nullptr, &used, nullptr);
    auto before = used;
    auto v = vector<vector<int>>(1);
    v[0] = vector<int>(1);
    v[0][0] = 5;
    rt_memory_info(nullptr, &used, nullptr);

    rt_kprintf("mem inc: %d\n", used - before);
}

void test_stl_array() {
    rt_uint32_t used;
    rt_memory_info(nullptr, &used, nullptr);
    auto before = used;
    auto v = array<array<int, 1>, 1>{};
    v[0][0] = 5;
    rt_memory_info(nullptr, &used, nullptr);

    rt_kprintf("mem inc: %d\n", used - before);
    rt_kprintf("size of v: %d\n", sizeof(v));
}

#include <config/app.hxx>
void test_stl_rtti_enum() {
    auto& t = typeid(State::Value);
    rt_kprintf("name: %s, hash: %08x\n", t.name(), t.hash_code());
}

#include <optional>
void test_optional() {
    auto test = optional<int>{5};
    rt_kprintf("sizeof: %d\n", sizeof(test));
}


template<class T, class U>
struct Response {
    int id;
    U data;
};

void test_stl_rtti_unused_temp() {
    struct Req1 {

    };

    struct Req2 {

    };

    using Res1 = Response<Req1, int>;
    using Res2 = Response<Req2, int>;
    rt_kprintf("[Res1] type name: %s, hash code: %08x\n", typeid(Res1).name(), typeid(Res1).hash_code());
    rt_kprintf("[Res2] type name: %s, hash code: %08x\n", typeid(Res2).name(), typeid(Res2).hash_code());
}

MSH_CMD_EXPORT(test_stl_vector, );
MSH_CMD_EXPORT(test_stl_array, );
MSH_CMD_EXPORT(test_stl_rtti_enum, );
MSH_CMD_EXPORT(test_optional, );
MSH_CMD_EXPORT(test_stl_rtti_unused_temp, );
#endif

