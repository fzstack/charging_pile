/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-04-19     imgcr       the first version
 */

#ifdef TEST_RETRY
#include <utilities/retry.hxx>
#include <rtthread.h>
#include <memory>
#include <utilities/shared_thread.hxx>
#include <components/rpc.hxx>
#include <config/co.hxx>

using namespace std;

auto veryLow = std::make_shared<SharedThread>(2048, 25, 10);

void test_retry_higher() {
    auto middle = Preset::SharedThread<Priority::Middle>::get();
    auto rpc = Preset::Rpc::get();
    Retry<int>::start([middle, val = 5] (auto fetch) mutable {
        --val;

        //以下代码可抽象为对f(val, fetch)的调用
        middle->exec([val, fetch]{ //跨线程的测试，重试直到值<=3则成功调用
            if(val <= 3) {
                fetch(val);
            } else {
                fetch(make_exception_ptr(runtime_error{""}));
            }
        });
    }).then([](auto v){ //处理成功调用的返回结果
        rt_kprintf("result: %d\n", v);
    }).except([]{
        rt_kprintf("retry failed\n");
    });

    rt_kprintf("other program\n");
}

void test_retry_same() {
    auto middle = Preset::SharedThread<Priority::Middle>::get();
    auto rpc = Preset::Rpc::get();
    Retry<int>::start([middle, val = 5] (auto fetch) mutable {
        --val;

        if(val <= 3) {
            fetch(val);
        } else {
            fetch(make_exception_ptr(runtime_error{""}));
        }
    }).then([](auto v){ //处理成功调用的返回结果
        rt_kprintf("result: %d\n", v);
    }).except([]{
        rt_kprintf("retry failed\n");
    });

    rt_kprintf("other program\n");
}

int init_very_low() {
    veryLow->start();
    return RT_EOK;
}

void test_retry_lower() {
    auto rpc = Preset::Rpc::get();
    Retry<int>::start([val = 5] (auto fetch) mutable {
        --val;

        //以下代码可抽象为对f(val, fetch)的调用
        veryLow->exec([val, fetch]{ //跨线程的测试，重试直到值<=3则成功调用
            if(val <= 3) {
                fetch(val);
            } else {
                fetch(make_exception_ptr(runtime_error{""}));
            }
        });
    }).then([](auto v){ //处理成功调用的返回结果
        rt_kprintf("result: %d\n", v);
    }).except([]{
        rt_kprintf("retry failed\n");
    });

    rt_kprintf("other program\n");
}


void test_retry_rpc() {
    auto thread = Preset::SharedThread<Priority::Middle>::get();
    auto rpc = Preset::Rpc::get();
    Retry<Void>::start([=](auto fetch){
        thread->exec([=]{
            rpc->invoke<Rpcs::Services::Query>({}, fetch);
        });
    }).except([]{
        rt_kprintf("query invoke failed\n");
    });
}

MSH_CMD_EXPORT(test_retry_higher,);
MSH_CMD_EXPORT(test_retry_same,);
MSH_CMD_EXPORT(test_retry_lower,);
MSH_CMD_EXPORT(test_retry_rpc,);
INIT_APP_EXPORT(init_very_low);

#endif
