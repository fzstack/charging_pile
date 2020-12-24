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
    Deliver(outer_t* outer, L&& f, id_t id): base_t(outer, id), f(std::forward<L>(f)), result() {

    }
private:
    void addTo(Signals<R(A...)>& signal) {
        signal += [this](Signals<void(R)> r, A... a){
            args = std::make_shared<std::tuple<A...>>(std::forward<A>(a)...);
            result = r;
            sendEvent();
            rt_kprintf("sending event, flag=%d\n", 1 << id);
        };
        rt_kprintf("deliver added to signal\n");
    }
public:
    virtual void invoke() override {
        auto r = std::apply(f, *args);
        rt_kprintf("deliver returning\n");
        //TODO: result可能有问题
        result(r);
    }
private:
    L f;
    Signals<void(R)> result;
    std::shared_ptr<std::tuple<A...>> args;
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
        //TODO: store args to variable
            args = std::make_shared<std::tuple<A...>>(std::forward<A>(a)...);
            sendEvent();
        };
    }
public:
    virtual void invoke() override {
        std::apply(f, *args);
    }
private:
    L f;
    std::shared_ptr<std::tuple<A...>> args;

};

#endif /* UTILITIES_DELIVER_HXX_ */
