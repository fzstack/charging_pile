/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-08-24     imgcr       the first version
 */
#ifndef APPLICATIONS2_SIGNALS_HPP_
#define APPLICATIONS2_SIGNALS_HPP_

#include <functional>
#include <list>
#include <utility>
#include "post.hxx"
#include "deliver.hxx"
#include <rtthread.h>

template<class T>
struct Signals;

template <class L, class F>
class Deliver;

//如果是DeliverProxy, 则不返回，转而调用onReturn, onReturn是function<void(R)>
template<class R, class ...P>
struct Signals<R(P...)> {
    void operator+=(std::function<void(Signals<void(R)> r, P...)> cb) {
        cbs.push_back(cb);
    }

    template<class L, class F>
    void operator +=(Deliver<L, F>& other) {
        rt_kprintf("sig cb before size: %d\n", cbs.size());
        other.addTo(*this);
        rt_kprintf("sig cb after size: %d\n", cbs.size());

    }

    //TODO: 返回对应的返回值的Signal, 那个Signal退出之后才开始调用回调，

    void operator()(Signals<void(R)> r, P ...p) {
        for(const auto& cb: cbs) {
            cb(r, p...);
        }
    }

    void operator()(std::function<void(R)> r, P ...p) {
        for(const auto& cb: cbs) {
            rt_kprintf("found a cb\n");
            auto signal = Signals<void(R)>();
            signal += r;
            cb(signal, p...);
        }
    }

    template<class L, class F>
    void operator()(Deliver<L, F>& r, P ...p) {
        for(const auto& cb: cbs) {
            auto signal = Signals<void(R)>();
            r.addTo(signal);
            cb(signal, p...);
        }
    }

    void operator()(P ...p) {
        for(const auto& cb: cbs) {
            cb(Signals<void(R)>(), p...);
        }
    }

    //onReturn由持有方 +=, 由回调调用
    //Signals<void(R)> onReturn;
    std::list<std::function<void(Signals<void(R)> r, P...)>> cbs;
};


template<class ...P>
struct Signals<void(P...)> {
    using func_type = std::function<void(P...)>;
    void operator+=(func_type cb) {
        cbs.push_back(cb);
    }

    void operator()(P ...p) const {
        for(const auto& cb: cbs) {
            cb(p...);
        }
    }

    std::list<func_type> cbs;
};

template<class F, class T>
auto signal(F&& f, T&& t) {
    return [f = std::forward<F>(f), t = std::forward<T>(t)](auto&&... args) {
        return (t->*f)(std::forward<decltype(args)>(args)...);
    };
}



#endif /* APPLICATIONS2_SIGNALS_HPP_ */
