/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-12-24     imgcr       the first version
 */
#ifndef UTILITIES_DELIVER_HXX_
#define UTILITIES_DELIVER_HXX_

#include "post.hxx"
#include "signals.hxx"
#include "deliver_base.hxx"
#include <rtthread.h>
#include "queue.hxx"
#include <tuple>

class Post;
template<class T>
struct Signals;

template <class L, class F = decltype(&L::operator())>
class Deliver;

template<class L, class R, class C, class... A>
class Deliver<L, R(C::*)(A...) const>: public DeliverBase {
    using base_t = DeliverBase;
    template<class T> friend struct Signals;
    using signals_cb_t = Signals<R(A...)>;
public:
    Deliver(outer_t* outer, L&& f): base_t(outer), f(std::forward<L>(f)) {

    }
private:
    void addTo(signals_cb_t& signal) {
        signal += [this](typename signals_cb_t::ret_sig_t r, A... a){
            if(getPollingThread() == rt_thread_self()) {
                rt_kprintf("at polling thread\n");
                auto result = f(a...);
                r(result);
            } else {
                rt_uint32_t used;
                rt_memory_info(RT_NULL, &used, RT_NULL);
                rt_kprintf("\033[94mbefore push queue, used mem: %d\n\033[0m", used);
                ctxQueue.push({r, {std::forward<A>(a)...}});
                rt_memory_info(RT_NULL, &used, RT_NULL);
                rt_kprintf("\033[94mafter push queue, used mem: %d\n\033[0m", used);
                enqueue();
            }
        };
    }
public:
    virtual void invoke() override {
        rt_uint32_t used;
        rt_memory_info(RT_NULL, &used, RT_NULL);
        rt_kprintf("\033[96mchecking queue, used mem: %d\n\033[0m", used);
        if(ctxQueue.empty()) return;
        {
            auto [result, args] = ctxQueue.pop();
            if(!ctxQueue.empty()) enqueue();
            rt_memory_info(RT_NULL, &used, RT_NULL);
            rt_kprintf("\033[96mcalling callback, used mem: %d\n\033[0m", used);
            auto r = std::apply(f, args);
            rt_memory_info(RT_NULL, &used, RT_NULL);
            rt_kprintf("\033[96mbefore call result func, used mem: %d\n\033[0m", used);
            result(std::forward<R>(r));
        }
        rt_memory_info(RT_NULL, &used, RT_NULL);
        rt_kprintf("\033[94mafter callback, used mem: %d\n\033[0m", used);

    }
private:
    L f;
    Queue<std::pair<typename signals_cb_t::ret_sig_t, std::tuple<A...>>> ctxQueue = {};
};

template<class L, class C, class... A>
class Deliver<L, void(C::*)(A...) const>: public DeliverBase {
    using base_t = DeliverBase;
    template<class T> friend struct Signals;
public:
    Deliver(outer_t* outer, L&& f): base_t(outer), f(std::forward<L>(f)) {

    }
private:
    void addTo(Signals<void(A...)>& signal) {
        signal += [this](A... a){
            if(getPollingThread() == rt_thread_self()) {
                rt_kprintf("at polling thread\n");
                f(a...);
            } else {
                rt_uint32_t used;
                rt_memory_info(RT_NULL, &used, RT_NULL);
                rt_kprintf("\033[94mpushing queue, used mem: %d\n\033[0m", used);
                argsQueue.push({a...});
                enqueue();
            }
        };
    }
public:
    virtual void invoke() override {
        rt_uint32_t used;
        rt_memory_info(RT_NULL, &used, RT_NULL);
        rt_kprintf("\033[94mchecking queue, used mem: %d\n\033[0m", used);
        if(argsQueue.empty()) return;
        {
            auto args = argsQueue.pop();
            if(!argsQueue.empty()) enqueue();
            rt_memory_info(RT_NULL, &used, RT_NULL);
            rt_kprintf("\033[94mcalling callback, used mem: %d\n\033[0m", used);
            std::apply(f, args);
        }
        rt_memory_info(RT_NULL, &used, RT_NULL);
        rt_kprintf("\033[94mafter callback, used mem: %d\n\033[0m", used);
    }
private:
    L f;
    Queue<std::tuple<A...>> argsQueue = {};
};

#endif /* UTILITIES_DELIVER_HXX_ */
