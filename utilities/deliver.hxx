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
public:
    Deliver(outer_t* outer, L&& f, id_t id): base_t(outer, id), f(std::forward<L>(f)) {

    }
private:
    void addTo(Signals<R(A...)>& signal) {
        signal += [this](Signals<void(R)> r, A... a){
            ctxQueue.push({r, {std::forward<A>(a)...}});
            sendEvent();
        };
    }
public:
    virtual void invoke() override {
        if(ctxQueue.empty()) return;
        auto [result, args] = ctxQueue.pop();
        if(!ctxQueue.empty()) sendEvent();
        auto r = std::apply(f, args);
        result(std::forward<R>(r));
    }
private:
    L f;
    Queue<std::pair<Signals<void(R)>, std::tuple<A...>>> ctxQueue = {};
};

template<class L, class C, class... A>
class Deliver<L, void(C::*)(A...) const>: public DeliverBase {
    using base_t = DeliverBase;
    template<class T> friend struct Signals;
public:
    Deliver(outer_t* outer, L&& f, id_t id): base_t(outer, id), f(std::forward<L>(f)) {

    }
private:
    void addTo(Signals<void(A...)>& signal) {
        signal += [this](A... a){
            argsQueue.push({std::forward<A>(a)...});
            sendEvent();
        };
    }
public:
    virtual void invoke() override {
        auto args = argsQueue.pop();
        std::apply(f, std::move(args));
    }
private:
    L f;
    Queue<std::tuple<A...>> argsQueue = {};
};

#endif /* UTILITIES_DELIVER_HXX_ */
