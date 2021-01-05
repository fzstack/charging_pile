/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-01-06     imgcr       the first version
 */

#include "thread_local.hxx"

#include <rtthread.h>
#include <stdexcept>
#include <memory>
#include <Thread.h>

using namespace std;
using namespace rtthread;

#ifdef TEST_THREAD_LOCAL

ThreadLocal<int> test;

void test_thread_local_global()
{
    class TestThread: public Thread {
        using Thread::Thread;
        virtual void run(void *parameter) override {
            test = 456;
            rt_event_send(event, 4);
            test = 123;
            rt_event_recv(event, 2, RT_EVENT_FLAG_CLEAR | RT_EVENT_FLAG_AND, RT_WAITING_FOREVER, nullptr);
            rt_kprintf("TT test: %d\n", *test);
            test = 233;
            rt_event_send(event, 1);
        }
    public:
        rt_event_t event = rt_event_create("T", RT_IPC_FLAG_FIFO);
    };

    auto testThr = TestThread{};
    test = 1;
    testThr.start();
    rt_event_recv(testThr.event, 4, RT_EVENT_FLAG_CLEAR | RT_EVENT_FLAG_AND, RT_WAITING_FOREVER, nullptr);
    rt_kprintf("main test: %d\n", *test);
    test = 2;
    rt_event_send(testThr.event, 2);
    rt_event_recv(testThr.event, 1, RT_EVENT_FLAG_CLEAR | RT_EVENT_FLAG_AND, RT_WAITING_FOREVER, nullptr);
    rt_kprintf("main test: %d\n", *test);

    testThr.join();
}

void test_thread_local_member()
{
    class TestThread: public Thread {
        using Thread::Thread;
        virtual void run(void *parameter) override {
            testMember = 456;
            rt_event_send(event, 4);
            testMember = 123;
            rt_event_recv(event, 2, RT_EVENT_FLAG_CLEAR | RT_EVENT_FLAG_AND, RT_WAITING_FOREVER, nullptr);
            rt_kprintf("TT test: %d\n", *testMember);
            testMember = 233;
            rt_event_send(event, 1);
        }
    public:
        ThreadLocal<int> testMember;
        rt_event_t event = rt_event_create("T", RT_IPC_FLAG_FIFO);
    };

    auto testThr = TestThread{};
    testThr.testMember = 1;
    testThr.start();
    rt_event_recv(testThr.event, 4, RT_EVENT_FLAG_CLEAR | RT_EVENT_FLAG_AND, RT_WAITING_FOREVER, nullptr);
    rt_kprintf("main test: %d\n", *testThr.testMember);
    testThr.testMember = 2;
    rt_event_send(testThr.event, 2);
    rt_event_recv(testThr.event, 1, RT_EVENT_FLAG_CLEAR | RT_EVENT_FLAG_AND, RT_WAITING_FOREVER, nullptr);
    rt_kprintf("main test: %d\n", *testThr.testMember);

    testThr.join();
}

void test_thread_local_init()
{
    class TestThread: public Thread {
        using Thread::Thread;
        virtual void run(void *parameter) override {
            rt_kprintf("TT test: %d\n", *testMember);
            testMember = 456;
            rt_event_send(event, 4);
            testMember = 123;
            rt_event_recv(event, 2, RT_EVENT_FLAG_CLEAR | RT_EVENT_FLAG_AND, RT_WAITING_FOREVER, nullptr);
            rt_kprintf("TT test: %d\n", *testMember);
            testMember = 233;
            rt_event_send(event, 1);
        }
    public:
        ThreadLocal<int> testMember = 5;
        rt_event_t event = rt_event_create("T", RT_IPC_FLAG_FIFO);
    };

    auto testThr = TestThread{};
    rt_kprintf("main test: %d\n", *testThr.testMember);
    testThr.testMember = 1;
    testThr.start();
    rt_event_recv(testThr.event, 4, RT_EVENT_FLAG_CLEAR | RT_EVENT_FLAG_AND, RT_WAITING_FOREVER, nullptr);
    rt_kprintf("main test: %d\n", *testThr.testMember);
    testThr.testMember = 2;
    rt_event_send(testThr.event, 2);
    rt_event_recv(testThr.event, 1, RT_EVENT_FLAG_CLEAR | RT_EVENT_FLAG_AND, RT_WAITING_FOREVER, nullptr);
    rt_kprintf("main test: %d\n", *testThr.testMember);

    testThr.join();
}


MSH_CMD_EXPORT(test_thread_local_global, );
MSH_CMD_EXPORT(test_thread_local_member, );
MSH_CMD_EXPORT(test_thread_local_init, );
#endif
