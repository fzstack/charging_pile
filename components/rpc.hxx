#pragma once

#include "packet.hxx"
#include <memory>
#include <set>
#include <map>
#include <utilities/event.hxx>
#include <Mutex.h>
#include <Lock.h>
#include <functional>
#include <string>
#include <optional>
#include <variant>
#include <vector>
#include <utilities/serialize_utilities.hxx>
#include <utilities/shared_thread.hxx>
#include <utilities/tiny_type_id.hxx>
#include <components/timer.hxx>
#include <utilities/shared_thread.hxx>

struct Void {

};

//#define LOG_RPC_PTR_RV
#define LOG_RPC_E_MAX_PARALLEL

//TODO: 调用超时

template<class T>
struct RpcTrait {
    using result_t = Void;
};

class Rpc {

public:
    Rpc(std::shared_ptr<Packet> packet, std::shared_ptr<SharedThread> thread);

private:
    using id_t = rt_uint8_t;

    template<class T>
    struct Request {
        id_t id;
        T data;
    };

    template<class T, class U = typename RpcTrait<T>::result_t>
    struct Response {
        id_t id;
        U data;
    };

    template<class T>
    struct Failure {
        id_t id;
        std::string msg;
    };

    template<class T>
    struct Release { //指针内的类型
        id_t id;
        T data;
    };

    template<class T>
    using result_t = typename RpcTrait<T>::result_t;

public:
    template<class T>
    using ivk_cb_param_t = std::variant<result_t<T>, std::exception_ptr>;

private:
    template<class T>
    using def_cb_param_t = std::variant<result_t<T>, std::exception_ptr>;

    struct Pending {
    public:
        Pending(int timeout): timeout(timeout) { }
        virtual void emitTimeout() = 0;
    public:
        const rt_tick_t createdTick = rt_tick_get();
        const rt_tick_t timeout;
    };

    template<class T>
    struct PendingImpl: public Pending {
        PendingImpl(std::function<void(ivk_cb_param_t<T>)> r, rt_tick_t timeout): Pending(timeout), r(r) {}
        virtual void emitTimeout() override {
#ifdef __cpp_exceptions
            try {
                throw timeout_error{"rpc invoke timeout"};
            } catch(std::exception& e) {
                r(std::current_exception());
            }
#endif
        }
        std::function<void(ivk_cb_param_t<T>)> r;
    };



public:



    template<class T>
    void invoke(T&& t, std::function<void(ivk_cb_param_t<T>)> r, rt_tick_t timeout = kTimeout) { //调用远程过程, 请不要在packet线程调用
        //TODO: 维护状态(用mutex)
        mutex.lock();
        if(registeredType.count(TypeId<T>::get()) == 0) {
            registeredType.insert(TypeId<T>::get());
            packet->on<Response<T>>([this](auto p){
                thread->exec([=]{
                    mutex.lock();
                    auto found = pendings.find(p->id);
                    if(found == pendings.end()) {
                        rt_kprintf("W: wild resp\n");
                        mutex.unlock();
                        return;
                    }

                    auto pending = std::static_pointer_cast<PendingImpl<T>>(found->second);
                    pendings.erase(p->id);
                    mutex.unlock();
                    p->data = handleInvokeResult(p->data, p->id);
                    pending->r(p->data);
                });
            });
            packet->on<Failure<T>>([this](auto p){
                thread->exec([=]{
                    mutex.lock();
                    auto found = pendings.find(p->id);
                    if(found == pendings.end()) {
                        rt_kprintf("W: wild resp\n");
                        mutex.unlock();
                        return;
                    }
                    auto pending = std::static_pointer_cast<PendingImpl<T>>(found->second);
                    pendings.erase(p->id);
                    mutex.unlock();
#ifdef __cpp_exceptions
                    try {
                        throw std::runtime_error{p->msg};
                    } catch(std::exception& e) {
                        pending->r(std::current_exception());
                    }
#endif
                });
            });
        }
        if(pendings.size() > kMaxParallel) {
            mutex.unlock();
#ifdef LOG_RPC_E_MAX_PARALLEL
            rt_kprintf("exceed max parallel when invoke %08x\n", TypeId<T>::get());
#endif
#ifdef __cpp_exceptions
            try {
                throw timeout_error{"timeout"};
            } catch(std::exception& e) {
                r(std::current_exception());
            }
#endif
            return;
        }

        auto pending = std::make_shared<PendingImpl<T>>(r, timeout);
        auto id = curId;
        curId++;
        pendings.insert({id, pending});
        mutex.unlock();
        packet->emit<Request<T>>({id, t});
    }


    template<class T>
    auto invoke(T&& t) -> result_t<T> { //调用远程过程, 请不要在packet线程调用
        auto event = Event{};
        auto data = std::make_shared<ivk_cb_param_t<T>>();
        invoke(std::forward<T>(t), [=](auto result) mutable {
            *data = result;
            event.set();
        });

        event.wait();
        auto pexcept = std::get_if<std::exception_ptr>(data.get());
        if(pexcept != nullptr) {
            std::rethrow_exception(*pexcept);
        }

        auto pdata = std::get_if<result_t<T>>(data.get());
        return *pdata;
    }

    template<class T>
    auto invoke(T& t) -> result_t<T> {
        return invoke(std::move(t));
    }

    template<class R> //R是指针类型
    auto handleInvokeResult(R&& r, id_t id) -> std::enable_if_t<SerUtilities::is_ptr_v<std::decay_t<R>>, std::decay_t<R>> {
        using ele_t = typename std::decay_t<R>::element_type;
#ifdef LOG_RPC_PTR_RV
        rt_kprintf("rpc managed ptr created\n");
#endif
        return std::shared_ptr<ele_t>(new ele_t{*r}, [=](auto p) { //在指针被销毁之后..
#ifdef LOG_RPC_PTR_RV
            rt_kprintf("rpc managed ptr releasing\n");
#endif
            packet->emit<Release<ele_t>>({id, *p}); //指针被销毁事件
            delete p;
        });
    }

    template<class R> //R非指针类型
    auto handleInvokeResult(R&& r, id_t id) -> std::enable_if_t<!SerUtilities::is_ptr_v<std::decay_t<R>>, std::decay_t<R>> {
        //DO NOTHING
        return r;
    }

    template<class T>
    void def(std::function<void(std::shared_ptr<T>, std::function<void(def_cb_param_t<T>)>)> cb) { //定义远程过程
        packet->on<Request<T>>([=](auto r) {
            thread->exec([=]{
                auto p = std::make_shared<T>(r->data);
                auto id = r->id;
                auto f = [this, id](def_cb_param_t<T> result){
#ifdef __cpp_exceptions
                    try {
#endif
                        if(auto err = std::get_if<std::exception_ptr>(&result)) std::rethrow_exception(*err);
                        auto data = std::get_if<result_t<T>>(&result);
                        handleRespResult(*data, id);
                        packet->emit<Response<T>>({id, *data});
#ifdef __cpp_exceptions
                    } catch(const std::exception& e) {
                        packet->emit<Failure<T>>({id, e.what()});
                    }
#endif
                };
                cb(p, f); //可能会被packet堵塞，所以在不同于middle的回调线程执行
            });
        });
        handleDefResult<result_t<T>>();
    }

    template<class T>
    auto def(std::function<result_t<T>(std::shared_ptr<T>)> cb) -> std::enable_if_t<!std::is_same_v<result_t<T>, Void>> {
        def<T>([=](auto p, auto r){
#ifdef __cpp_exceptions
            try {
#endif
                r(cb(p));
#ifdef __cpp_exceptions
            } catch(const std::exception& e) {
                r(std::current_exception());
            }
#endif
        });
    }

    template<class T>
    auto def(std::function<void(std::shared_ptr<T>)> cb) -> std::enable_if_t<std::is_same_v<result_t<T>, Void>> {
        def<T>([=](auto p, auto r){
            try {
                cb(p);
                r({});
            } catch(const std::exception& e) {
                r(std::current_exception());
            }
        });
    }

    template<class R>
    auto handleDefResult() -> std::enable_if_t<SerUtilities::is_ptr_v<std::decay_t<R>>> {
        using ele_t = typename R::element_type;
        packet->on<Release<ele_t>>([=](auto r){ //r是RelImpl的指针
            //查找表，看看id为xx的记录是否存在
            auto found = ptrHolds.find(r->id);
            if(found != ptrHolds.end()) {
                *std::reinterpret_pointer_cast<ele_t>(found->second) = r->data;
                ptrHolds.erase(found);
            }
        });
    }

    template<class R>
    auto handleDefResult() -> std::enable_if_t<!SerUtilities::is_ptr_v<std::decay_t<R>>> {
        //DO NOTHING
    }


    template<class R>
    auto handleRespResult(R&& r, id_t id) -> std::enable_if_t<SerUtilities::is_ptr_v<std::decay_t<R>>> {
        ptrHolds[id] = r;
    }

    template<class R>
    auto handleRespResult(R&& r, id_t id) -> std::enable_if_t<!SerUtilities::is_ptr_v<std::decay_t<R>>> {
        //DO NOTHING
    }

private:


private:
    std::shared_ptr<Packet> packet;
    std::shared_ptr<SharedThread> thread;
    std::set<std::size_t> registeredType = {};
    std::map<id_t, std::shared_ptr<Pending>> pendings;
    std::map<id_t, std::shared_ptr<void>> ptrHolds;
    id_t curId = 0;
    rtthread::Mutex mutex = {kMutex};
    Timer timer = {100, "rpc"};
    static constexpr int kMaxParallel = 2;
    static constexpr int kTimeout = 10 * 1000;
    static const char* kMutex;
};

#include <utilities/singleton.hxx>
namespace Preset {
class Rpc: public Singleton<Rpc>, public ::Rpc {
    friend singleton_t;
    Rpc(): ::Rpc(Packet::get(), SharedThread<Priority::Middle>::get()) { }
};
}
