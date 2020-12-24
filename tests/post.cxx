/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-12-23     imgcr       the first version
 */

#include "post.hxx"
#include <utilities/signals.hxx>


#define LOG_TAG "test.post"
#define LOG_LVL LOG_LVL_DBG
#include <ulog.h>

using namespace std;

#ifdef TEST_POST
static void test_post() {
    auto post = Post();

    auto testSig = Signals<string(string, int)>();

    testSig += post([](string x, int y) -> string {
        //在本线程中处理计算并返回
        return x + to_string(y);
    });

    ///NOTE: 在其他线程调用
    //处理返回值(构造函数中添加)
    testSig.onReturn += [](auto result) {
        LOG_D("result is %d", result.c_str());
    };
    //在其他任意进程发起回调调用
    testSig("233", 3);


//    for(auto i = 0; i < 5; i++) {
//        testSig("233", i) += [i](auto r) {
//            LOG_D("233+%d=%s", i, r.c_str());
//        };
//    }

    ///NOTE: 在本线程中调用
    post.poll();
}


MSH_CMD_EXPORT(test_post, );
#endif

