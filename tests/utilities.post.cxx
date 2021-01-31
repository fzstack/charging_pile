/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-12-23     imgcr       the first version
 */
#ifdef TEST_POST
#include <utilities/post.hxx>
#include <utilities/signals.hxx>
#include <string>
#include <Thread.h>
#include <utilities/json.hxx>
#include <vector>
#include <numeric>

#define LOG_TAG "test.post"
#define LOG_LVL LOG_LVL_DBG
#include <ulog.h>

using namespace std;
using namespace rtthread;

//TODO: json移动构造器可能存在问题
static void test_post_void_ret() {
    using signals_t = Signals<void(int, int)>;
    class TestThread: public Thread {
    public:
        TestThread(signals_t& signal): Thread(), signal(signal) { }
        virtual void run(void *p) override {
            auto post = Post();
            signal += post([](signals_t::param_t<0> a, signals_t::param_t<1> b) -> void {
                rt_kprintf("a: %d, b: %d\n", a, b);
                return;
            });

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
    signal(2, 6);
    test.join();
}

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

//TODO: json移动构造器可能存在问题
static void test_post_json() {
    constexpr int kTimes = 5;
    using signals_t = Signals<Json(Json, Json)>;
    class TestThread: public Thread {
    public:
        TestThread(signals_t& signal): Thread(), signal(signal) { }
        virtual void run(void *p) override {
            auto post = Post();
            signal += post([](signals_t::param_t<0> a, signals_t::param_t<1> b) -> signals_t::result_t {
                return {
                    {"a", a},
                    {"b", b},
                };
            });

            emitRegEvent();
            for(auto i = 0; i < kTimes; i++) {
                post.poll(PollType::OneShot);
            }
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
    for(auto i = 0; i < kTimes; i++) {
        signal(post([=](signals_t::result_t result){
            rt_kprintf("{%08x}, a=%d, b=%d, result=%s\n", rt_thread_self(), 2, i, to_string(result).c_str());
        }), 2, i);
    }

    for(auto i = 0; i < kTimes; i++) {
        post.poll(PollType::OneShot);
    }
    test.join();
}

static void test_post_copy_param_ret() {
    using signals_t = Signals<int(vector<int>)>;
    class TestThread: public Thread {
    public:
        TestThread(signals_t& signal): Thread(), signal(signal) { }
        virtual void run(void *p) override {
            auto post = Post();
            rt_kprintf("{%08x}post created\n", rt_thread_self());
            signal += post([](vector<int> input) {
                auto sum = accumulate(input.begin(), input.end(), 0);
                return sum;
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
    signal(post([](signals_t::result_t result){
        rt_kprintf("{%08x}, result=%d\n", rt_thread_self(), result);
    }), {2, 3, 4, 5});
    rt_kprintf("{%08x}test sig called\n", rt_thread_self());
    post.poll(PollType::OneShot);
    test.join();
}


static void test_post_closure() {
    constexpr int kTimes = 5;
    using signals_t = Signals<int(int, int)>;
        class TestThread: public Thread {
        public:
            TestThread(signals_t& signal): Thread(), signal(signal) { }
            virtual void run(void *p) override {
                auto post = Post();
                rt_kprintf("{%08x}post created\n", rt_thread_self());
                signal += post([](int a, int b) {
                    rt_kprintf("{%08x}, a=%d, b=%d\n", rt_thread_self(), a, b);
                    return a + b;
                });
                rt_kprintf("{%08x}sig callback reged\n", rt_thread_self());

                emitRegEvent();

                for(auto i = 0; i < kTimes; i++) {
                    post.poll(PollType::OneShot);
                }
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
        for(auto i = 0; i < kTimes; i++) {
            signal(post([i](int result){
                rt_kprintf("{%08x}, a=2, b=%d, result=%d\n", rt_thread_self(), i, result);
            }), 2, i);
        }

        rt_kprintf("{%08x}test sig called\n", rt_thread_self());
        for(auto i = 0; i < kTimes; i++) {
            post.poll(PollType::OneShot);
        }
        test.join();
}

//在同线程处理返回值
static void test_post_ret_same_thread() {
    using signals_t = Signals<Json(Json, Json)>;
    ///已知bug: 优先级大于tshell会导致直接崩溃，优先级小于tshell会导致内存泄漏
    constexpr int threadPrio = 25;
    class TestThread: public Thread {
    public:
        TestThread(): Thread(2048, threadPrio) { }
        virtual void run(void *p) override {
            //在本线程处理回调的返回值

            //signal被调用、在本线程
            signal(post([](Json result){
                rt_kprintf("result: %s\n", to_string(result).c_str());
            }), 4, 5);
            rt_kprintf("push deliver with Json arg to queue\n");
            post.poll(PollType::OneShot);
        }

    public:
        Post post = {};
        signals_t signal;
    };
    auto test = TestThread();
    test.signal += [](Json a, Json b) -> Json {
        rt_kprintf("a: %s, b: %s\n", to_string(a).c_str(), to_string(b).c_str());
        return {
            {"a", a},
            {"b", b},
        };
    };
    test.start();
    test.join();

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


MSH_CMD_EXPORT(test_post_void_ret, );
MSH_CMD_EXPORT(test_post_async_ret, );
MSH_CMD_EXPORT(test_post_copy_param_ret, );
MSH_CMD_EXPORT(test_post_json, );
MSH_CMD_EXPORT(test_post_closure, );
MSH_CMD_EXPORT(test_post_ret_same_thread, );
INIT_APP_EXPORT(init_test_post);
#endif

