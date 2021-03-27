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

struct Void {

};

#define LOG_RPC_CB

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

    struct Pending {
        Event event;
        std::optional<std::string> failureMsg = {};
    protected:
        virtual void v() {}
    };

    template<class U>
    struct PendingImpl: public Pending {
        std::shared_ptr<U> data;
    };

    template<class T>
    using result_t = typename RpcTrait<T>::result_t;

public:

    template<class T>
    using cb_param_t = std::variant<result_t<T>, std::exception_ptr>;

    template<class T>
    auto invoke(T&& t, std::function<void(cb_param_t<T>)> r) { //调用远程过程, 请不要在packet线程调用
        //TODO: 维护状态


    }


    template<class T>
    auto invoke(T&& t) -> result_t<T> { //调用远程过程, 请不要在packet线程调用
        mutex.lock();
        if(registeredType.count(typeid(T).hash_code()) == 0) {
            registeredType.insert(typeid(T).hash_code());
            packet->on<Response<T>>([this](auto p){
                auto guard = rtthread::Lock(mutex);
                auto found = pendings.find(p->id);
                if(found == pendings.end()) {
                    rt_kprintf("W: wild resp\n");
                    return;
                }
                auto impl = std::dynamic_pointer_cast<PendingImpl<result_t<T>>>(found->second);
                impl->data = std::make_shared<result_t<T>>(p->data);
                impl->event.set();
            });
            packet->on<Failure<T>>([this](auto p){
                auto guard = rtthread::Lock(mutex);
                auto found = pendings.find(p->id);
                if(found == pendings.end()) {
                    rt_kprintf("W: wild resp\n");
                    return;
                }
                auto pending = found->second;
                pending->failureMsg = p->msg;
                pending->event.set();
            });
        }
        auto pending = std::make_shared<PendingImpl<result_t<T>>>();
        auto id = curId;
        curId++;
        pendings.insert({id, pending});
        packet->emit<Request<T>>({id, t});
        mutex.unlock();
        pending->event.wait();
        auto guard = rtthread::Lock{mutex};
        pendings.erase(id);
        if(pending->failureMsg)
            throw std::runtime_error{*pending->failureMsg};
        return handleInvokeResult(*pending->data, id);
    }

    template<class T>
    auto invoke(T& t) -> result_t<T> {
        return invoke(std::move(t));
    }

    template<class R> //R是指针类型
    auto handleInvokeResult(R&& r, id_t id) -> std::enable_if_t<SerUtilities::is_ptr_v<std::decay_t<R>>, std::decay_t<R>> {
        using ele_t = typename std::decay_t<R>::element_type;
        return std::shared_ptr<ele_t>(new ele_t{*r}, [=](auto p) { //在指针被销毁之后..
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
    void def(std::function<void(std::shared_ptr<T>, std::function<void(cb_param_t<T>)>)> cb) { //定义远程过程
        packet->on<Request<T>>([=](auto r) {
            auto p = std::make_shared<T>(r->data);
            auto id = r->id;
            auto f = [this, id](cb_param_t<T> result){
                try {
                    if(auto err = std::get_if<std::exception_ptr>(&result)) std::rethrow_exception(*err);
                    auto data = std::get_if<result_t<T>>(&result);
                    handleRespResult(*data, id);
                    packet->emit<Response<T>>({id, *data});
                } catch(const std::exception& e) {
                    packet->emit<Failure<T>>({id, e.what()});
                }
            };
#ifdef LOG_RPC_CB
            rt_kprintf("try invoke rpc cb...\n");
#endif
            thread->exec([=](){
#ifdef LOG_RPC_CB
            rt_kprintf("invoking rpc cb @%s...\n", rt_thread_self()->name);
#endif
                cb(p, f); //可能会被packet堵塞，所以在不同于middle的回调线程执行
#ifdef LOG_RPC_CB
            rt_kprintf("rpc cb invoked @%s\n", rt_thread_self()->name);
#endif
            });
        });
        handleDefResult<result_t<T>>();
    }

    template<class T>
    auto def(std::function<result_t<T>(std::shared_ptr<T>)> cb) -> std::enable_if_t<!std::is_same_v<result_t<T>, Void>> {
        def<T>([=](auto p, auto r){
            try {
                r(cb(p));
            } catch(const std::exception& e) {
                r(std::current_exception());
            }
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
    static const char* kMutex;
};

#include <utilities/singleton.hxx>
namespace Preset {
class Rpc: public Singleton<Rpc>, public ::Rpc {
    friend singleton_t;
    Rpc(): ::Rpc(Packet::get(), SharedThread<Priority::Rpc>::get()) { }
};
}

