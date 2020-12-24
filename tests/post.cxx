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
#include <string>
#include <applications/json.hxx>

#define LOG_TAG "test.post"
#define LOG_LVL LOG_LVL_DBG
#include <ulog.h>

using namespace std;

#ifdef TEST_POST

static void test_post_async_ret() {
    class Test{
    public:
        Signals<int(int, int)> testSig = {};
        shared_ptr<rt_event> event = shared_ptr<rt_event>(rt_event_create("tt", RT_IPC_FLAG_FIFO), [](auto p) {
            rt_event_delete(p);
        });
    } test;

    auto testThread = rt_thread_create("test", [](auto p) {
        auto post = Post();
        auto t = (Test*)p;
        rt_kprintf("{%08x}post created\n", rt_thread_self());

        t->testSig += post([](int a, int b) {
            rt_kprintf("{%08x}, a=%d, b=%d\n", rt_thread_self(), a, b);
            return a + b;
        });
        rt_kprintf("{%08x}sig callback reged\n", rt_thread_self());

        rt_event_send(t->event.get(), 1);
        post.poll(PollType::OneShot);
        for(;;);
    }, &test, 2048, 30, 5);
    rt_thread_startup(testThread);

    rt_event_recv(test.event.get(), 1, RT_EVENT_FLAG_AND | RT_EVENT_FLAG_CLEAR, RT_WAITING_FOREVER, nullptr);

    auto post = Post();
    test.testSig(post([](int result){
        rt_kprintf("{%08x}, result=%d\n", rt_thread_self(), result);
    }), 2, 3);
    rt_kprintf("{%08x}test sig called\n", rt_thread_self());
    post.poll(PollType::OneShot);
}

static void test_post_copy_param_ret() {
    class Test{
    public:
        Signals<string(string, int)> testSig = {};
        shared_ptr<rt_event> event = shared_ptr<rt_event>(rt_event_create("tt", RT_IPC_FLAG_FIFO), [](auto p) {
            rt_event_delete(p);
        });
    } test;

    auto testThread = rt_thread_create("test", [](auto p) {
        auto post = Post();
        auto t = (Test*)p;
        rt_kprintf("{%08x}post created\n", rt_thread_self());

        t->testSig += post([](string a, int b) {
            rt_kprintf("{%08x}, a=%s, b=%d\n", rt_thread_self(), a.c_str(), b);
            return a + to_string(b);
        });
        rt_kprintf("{%08x}sig callback reged\n", rt_thread_self());

        rt_event_send(t->event.get(), 1);
        post.poll(PollType::OneShot);
        for(;;);
    }, &test, 2048, 30, 5);
    rt_thread_startup(testThread);

    rt_event_recv(test.event.get(), 1, RT_EVENT_FLAG_AND | RT_EVENT_FLAG_CLEAR, RT_WAITING_FOREVER, nullptr);

    auto post = Post();
    test.testSig(post([](string result){
        rt_kprintf("{%08x}, result=%s\n", rt_thread_self(), result.c_str());
    }), "2", 3);
    rt_kprintf("{%08x}test sig called\n", rt_thread_self());
    post.poll(PollType::OneShot);
}

static void test_post_json() {
    class Test{
    public:
        Signals<Json(Json, Json)> testSig = {};
        shared_ptr<rt_event> event = shared_ptr<rt_event>(rt_event_create("tt", RT_IPC_FLAG_FIFO), [](auto p) {
            rt_event_delete(p);
        });
    } test;

    auto testThread = rt_thread_create("test", [](auto p) {
        auto post = Post();
        auto t = (Test*)p;
        rt_kprintf("{%08x}post created\n", rt_thread_self());

        t->testSig += post([](Json a, Json b) -> Json {
            rt_kprintf("{%08x}, a=%s, b=%s\n", rt_thread_self(), to_string(a).c_str(), to_string(b).c_str());
            return {
                {"a", a},
                {"b", b},
            };
        });
        rt_kprintf("{%08x}sig callback reged\n", rt_thread_self());

        rt_event_send(t->event.get(), 1);
        post.poll(PollType::OneShot);
        for(;;);
    }, &test, 2048, 30, 5);
    rt_thread_startup(testThread);

    rt_event_recv(test.event.get(), 1, RT_EVENT_FLAG_AND | RT_EVENT_FLAG_CLEAR, RT_WAITING_FOREVER, nullptr);

    auto post = Post();
    test.testSig(post([](Json result){
        rt_kprintf("{%08x}, result=%s\n", rt_thread_self(), to_string(result).c_str());
    }), "2", 3);
    rt_kprintf("{%08x}test sig called\n", rt_thread_self());
    post.poll(PollType::OneShot);
}

static void test_post_closure() {
    class Test{
    public:
        Signals<Json(Json, Json)> testSig = {};
        shared_ptr<rt_event> event = shared_ptr<rt_event>(rt_event_create("tt", RT_IPC_FLAG_FIFO), [](auto p) {
            rt_event_delete(p);
        });
    } test;

    auto testThread = rt_thread_create("test", [](auto p) {
        auto post = Post();
        auto t = (Test*)p;
        rt_kprintf("{%08x}post created\n", rt_thread_self());

        t->testSig += post([](Json a, Json b) -> Json {
            rt_kprintf("{%08x}, a=%s, b=%s\n", rt_thread_self(), to_string(a).c_str(), to_string(b).c_str());
            return {
                {"a", a},
                {"b", b},
            };
        });
        rt_kprintf("{%08x}sig callback reged\n", rt_thread_self());

        rt_event_send(t->event.get(), 1);
        post.poll(PollType::OneShot);
        for(;;);
    }, &test, 2048, 30, 5);
    rt_thread_startup(testThread);

    rt_event_recv(test.event.get(), 1, RT_EVENT_FLAG_AND | RT_EVENT_FLAG_CLEAR, RT_WAITING_FOREVER, nullptr);

    auto post = Post();
    test.testSig(post([](Json result){
        rt_kprintf("{%08x}, result=%s\n", rt_thread_self(), to_string(result).c_str());
    }), "2", 3);
    rt_kprintf("{%08x}test sig called\n", rt_thread_self());
    post.poll(PollType::OneShot);
}

//TODO: 检查Post对象是否有效
//TODO: 检查event是否有效

//互斥锁保护状态
//TODO: 队列

static int init_test_post() {

    return RT_EOK;
}


MSH_CMD_EXPORT(test_post_async_ret, );
MSH_CMD_EXPORT(test_post_copy_param_ret, );
MSH_CMD_EXPORT(test_post_json, );
INIT_APP_EXPORT(init_test_post);
#endif

