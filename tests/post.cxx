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
#include <Thread.h>

#define LOG_TAG "test.post"
#define LOG_LVL LOG_LVL_DBG
#include <ulog.h>

using namespace std;
using namespace rtthread;

#ifdef TEST_POST

static void test_post_async_ret() {
    class TestThread: public Thread {
    public:
        TestThread(Signals<int(int, int)>& signal): Thread(), signal(signal) { }
        virtual void run(void *p) override {
            auto post = Post();
            rt_kprintf("{%08x}post created\n", rt_thread_self());
            signal += post([](int a, int b) {
                rt_kprintf("{%08x}, a=%d, b=%d\n", rt_thread_self(), a, b);
                return a + b;
            });
            rt_kprintf("{%08x}sig callback reged\n", rt_thread_self());

            emitRegEvent();
            post.poll(PollType::OneShot);
        }
        void waitReg() {
            rt_event_recv(event.get(), 1, RT_EVENT_FLAG_AND | RT_EVENT_FLAG_CLEAR, RT_WAITING_FOREVER, nullptr);
        }
    private:
        void emitRegEvent() {
            rt_event_send(event.get(), 1);
        }
    private:
        Signals<int(int, int)>& signal;
        shared_ptr<rt_event> event = shared_ptr<rt_event>(rt_event_create("tt", RT_IPC_FLAG_FIFO), [](auto p) {
            rt_event_delete(p);
        });
    };

    Signals<int(int, int)> signal;
    auto test = TestThread(signal);
    test.start();
    test.waitReg();
    auto post = Post();
    signal(post([](int result){
        rt_kprintf("{%08x}, result=%d\n", rt_thread_self(), result);
    }), 2, 3);
    rt_kprintf("{%08x}test sig called\n", rt_thread_self());
    post.poll(PollType::OneShot);
    test.join();
}

static void test_post_json() {
    using signals_t = Signals<Json(Json, Json)>;
    class TestThread: public Thread {
    public:
        TestThread(signals_t& signal): Thread(), signal(signal) { }
        virtual void run(void *p) override {
            auto post = Post();
            rt_kprintf("{%08x}post created\n", rt_thread_self());
            signal += post([](Json a, Json b) -> Json {
                rt_kprintf("{%08x}, a=%s, b=%s\n", rt_thread_self(), to_string(a).c_str(), to_string(b).c_str());
                return {
                    {"a", a},
                    {"b", b},
                };
            });
            rt_kprintf("{%08x}sig callback reged\n", rt_thread_self());

            emitRegEvent();
            post.poll(PollType::OneShot);
        }
        void waitReg() {
            rt_event_recv(event.get(), 1, RT_EVENT_FLAG_AND | RT_EVENT_FLAG_CLEAR, RT_WAITING_FOREVER, nullptr);
        }
    private:
        void emitRegEvent() {
            rt_event_send(event.get(), 1);
        }
    private:
        signals_t& signal;
        shared_ptr<rt_event> event = shared_ptr<rt_event>(rt_event_create("tt", RT_IPC_FLAG_FIFO), [](auto p) {
            rt_event_delete(p);
        });
    };

    signals_t signal;
    auto test = TestThread(signal);
    test.start();
    test.waitReg();
    auto post = Post();
    signal(post([](Json result){
        rt_kprintf("{%08x}, result=%s\n", rt_thread_self(), to_string(result).c_str());
    }), "2", 3);
    rt_kprintf("{%08x}test sig called\n", rt_thread_self());
    post.poll(PollType::OneShot);
    test.join();
}

static void test_post_copy_param_ret() {
    using signals_t = Signals<string(string, int)>;
    class TestThread: public Thread {
    public:
        TestThread(signals_t& signal): Thread(), signal(signal) { }
        virtual void run(void *p) override {
            auto post = Post();
            rt_kprintf("{%08x}post created\n", rt_thread_self());
            signal += post([](string a, int b) {
                rt_kprintf("{%08x}, a=%s, b=%d\n", rt_thread_self(), a.c_str(), b);
                return a + to_string(b);
            });
            rt_kprintf("{%08x}sig callback reged\n", rt_thread_self());

            emitRegEvent();
            post.poll(PollType::OneShot);
        }
        void waitReg() {
            rt_event_recv(event.get(), 1, RT_EVENT_FLAG_AND | RT_EVENT_FLAG_CLEAR, RT_WAITING_FOREVER, nullptr);
        }
    private:
        void emitRegEvent() {
            rt_event_send(event.get(), 1);
        }
    private:
        signals_t& signal;
        shared_ptr<rt_event> event = shared_ptr<rt_event>(rt_event_create("tt", RT_IPC_FLAG_FIFO), [](auto p) {
            rt_event_delete(p);
        });
    };

    signals_t signal;
    auto test = TestThread(signal);
    test.start();
    test.waitReg();
    auto post = Post();
    signal(post([](string result){
        rt_kprintf("{%08x}, result=%s\n", rt_thread_self(), result.c_str());
    }), "2", 3);
    rt_kprintf("{%08x}test sig called\n", rt_thread_self());
    post.poll(PollType::OneShot);
    test.join();
}


static void test_post_closure() {
    class Test{
    public:
        Signals<Json(Json, Json)> testSig = {};
        shared_ptr<rt_event> event = shared_ptr<rt_event>(rt_event_create("tt", RT_IPC_FLAG_FIFO), [](auto p) {
            rt_event_delete(p);
        });
    } test;

    auto testThread = shared_ptr<rt_thread>(rt_thread_create("test", [](auto p) {
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
    }, &test, 2048, 30, 5), [](auto p ) {
        rt_thread_delete(p);
    });
    rt_thread_startup(testThread.get());

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
//TODO: 同线程测试
//TODO: 内存泄漏问题

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

