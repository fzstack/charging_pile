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

template<class L, class F>
void runOn(std::shared_ptr<Deliver<L, F>>&& r);

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
        try {
            auto r = std::apply(f, args);
            result(std::forward<R>(r));
        } catch(const std::exception& e) {
            result(std::current_exception());
        }

        if(type == DeliverType::ShortTerm)
            dispose();
    }
private:
    L f;
    Queue<std::tuple<typename signals_cb_t::ret_sig_t, std::tuple<A...>, DeliverType>> ctxQueue = {};
};

template<class L, class C, class... A>
class Deliver<L, void(C::*)(A...) const>: public DeliverBase {
    using base_t = DeliverBase;
    template<class T> friend struct Signals;
    template<class LL, class LF> friend void runOn(std::shared_ptr<Deliver<LL, LF>>&& r);
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
        try {
            std::apply(f, args);
            result({});
        } catch(const std::exception& e) {
            result({std::current_exception()});
        }
        if(type == DeliverType::ShortTerm)
            dispose();
    }
private:
    L f;
    Queue<std::tuple<typename signals_cb_t::ret_sig_t, std::tuple<A...>, DeliverType>> ctxQueue = {};
};

template<class L, class C, class... A>
class Deliver<L, void(C::*)(A...)>: public DeliverBase {
    using base_t = DeliverBase;
    template<class T> friend struct Signals;
    template<class LL, class LF> friend void runOn(std::shared_ptr<Deliver<LL, LF>>&& r);
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
        try {
            std::apply(f, args);
            result({});
        } catch(const std::exception& e) {
            result({std::current_exception()});
        }
        if(type == DeliverType::ShortTerm)
            dispose();
    }
private:
    L f;
    Queue<std::tuple<typename signals_cb_t::ret_sig_t, std::tuple<A...>, DeliverType>> ctxQueue = {};
};

#endif /* UTILITIES_DELIVER_HXX_ */
