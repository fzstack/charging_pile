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

enum class DeliverType {
    LongTerm,
    ShortTerm,
};

template<class L, class R, class C, class... A>
class Deliver<L, R(C::*)(A...) const>: public DeliverBase {
    using base_t = DeliverBase;
    template<class T> friend struct Signals;
    using signals_cb_t = Signals<R(A...)>;
public:
    Deliver(outer_t* outer, L&& f): base_t(outer), f(std::forward<L>(f)) {

    }
private:
    void addTo(signals_cb_t& signal, DeliverType type = DeliverType::LongTerm) {
        signal += [this, type](typename signals_cb_t::ret_sig_t r, A&&... a){
            ctxQueue.push({r, {std::forward<A>(a)...}, type});
            enqueue();
        };
    }
public:
    virtual void invoke() override {
        if(ctxQueue.empty()) return;
        auto [result, args, type] = ctxQueue.pop();
        auto r = std::apply(f, args);
        result(std::forward<R>(r));
        if(type == DeliverType::ShortTerm)
            dispose();
    }
private:
    L f;
    Queue<std::pair<typename signals_cb_t::ret_sig_t, std::tuple<A...>>> ctxQueue = {};
};

template<class L, class C, class... A>
class Deliver<L, void(C::*)(A...) const>: public DeliverBase {
    using base_t = DeliverBase;
    template<class T> friend struct Signals;
    using signals_cb_t = Signals<void(A...)>;
public:
    Deliver(outer_t* outer, L&& f): base_t(outer), f(std::forward<L>(f)) {

    }
private:
    void addTo(signals_cb_t& signal, DeliverType type = DeliverType::LongTerm) {
        signal += [this, type](typename signals_cb_t::ret_sig_t r, A&&... a){
            ctxQueue.push({r, {std::forward<A>(a)...}, type});
            enqueue();
        };
    }
public:
    virtual void invoke() override {
        if(ctxQueue.empty()) return;
        auto [result, args, type] = ctxQueue.pop();
        std::apply(f, args);
        result();
        if(type == DeliverType::ShortTerm)
            dispose();
    }
private:
    L f;
    Queue<std::tuple<typename signals_cb_t::ret_sig_t, std::tuple<A...>, DeliverType>> ctxQueue = {};
};

#endif /* UTILITIES_DELIVER_HXX_ */
