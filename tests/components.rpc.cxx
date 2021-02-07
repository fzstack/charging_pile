/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-02-05     imgcr       the first version
 */


#include <components/rpc.hxx>
#include <utilities/cmd.hxx>

#ifdef TEST_RPC

#define LOG_TAG "test.rpc"
#define LOG_LVL LOG_LVL_DBG
#include <ulog.h>

using namespace std;


struct Add {
    int a, b;
};

template<>
struct RpcTrait<Add> {
    using result_t = int;
};

struct Delay {
    int ms;
};


void test_rpc_add(int argc, char** argv) {
    ASSERT_MIN_NARGS(3);
    auto rpc = Preset::Rpc::get();
    auto a = atoi(argv[1]);
    auto b = atoi(argv[2]);
    auto v = rpc->invoke<Add>({a, b});
    rt_kprintf("result: %d\n", v);
}

int init_test_rpc() {
    auto rpc = Preset::Rpc::get();
    rpc->def<Add>([](auto p){
        return p->a + p->b;
    });
    return RT_EOK;
}

MSH_CMD_EXPORT(test_rpc_add, );
INIT_APP_EXPORT(init_test_rpc);
#endif

