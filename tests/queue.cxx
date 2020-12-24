/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-12-25     imgcr       the first version
 */

#include "queue.hxx"
#include <rtthread.h>
#include <Thread.h>

#define LOG_TAG "test.queue"
#define LOG_LVL LOG_LVL_DBG
#include <ulog.h>

using namespace std;
using namespace rtthread;

#ifdef TEST_QUEUE

class IntSPTestThread: public Thread {
public:
    IntSPTestThread(Queue<shared_ptr<int>>& queue): Thread(), queue(queue) {

    }
    virtual void run(void *p) override {
        while(true) {
            auto value = queue.pop();
            rt_kprintf("recv: %d\n", *value);
        }
    }

private:
    Queue<shared_ptr<int>>& queue;
};

Queue<shared_ptr<int>> intTestQueue;
IntSPTestThread intTestThread(intTestQueue);

static void test_queue_int() {
    for(auto i = 0; i < 10; i++) {
        auto value = make_shared<int>(i);
        intTestQueue.push(value);
        rt_kprintf("send %d\n", *value);
    }
}

static int init_test_queue() {
    intTestThread.start();
    return RT_EOK;
}

MSH_CMD_EXPORT(test_queue_int, );
INIT_APP_EXPORT(init_test_queue);
#endif

