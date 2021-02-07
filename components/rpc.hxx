/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-02-05     imgcr       the first version
 */
#pragma once

#include "packet.hxx"
#include <memory>
#include <set>
#include <map>
#include <utilities/event.hxx>
#include <Mutex.h>
#include <Lock.h>
#include <functional>

struct Void {

};

template<class T>
struct RpcTrait {
    using result_t = Void;
};

class Rpc {

public:
    Rpc(std::shared_ptr<Packet> packet);

private:
    using id_t = rt_uint32_t;

    template<class T>
    struct Request {
        id_t id;
        T data;
    };

    template<class T>
    struct Response {
        id_t id;
        typename RpcTrait<T>::result_t data;
    };

    struct Pending {
        Event event;
    protected:
        virtual void v() {}
    };

    template<class U>
    struct PendingImpl: public Pending {
        std::shared_ptr<U> data;
    };

public:
    template<class T>
    auto invoke(T&& t) -> typename RpcTrait<T>::result_t { //调用远程过程, 请不要在packet线程调用
        mutex.lock();
        using result_t = decltype(invoke(t));
        if(registeredType.count(typeid(T).hash_code()) == 0) {
            registeredType.insert(typeid(T).hash_code());
            packet->on<Response<T>>([this](auto p){
                auto guard = rtthread::Lock(mutex);
                auto found = pendings.find(p->id);
                if(found == pendings.end()) {
                    rt_kprintf("W: wild resp\n");
                    return;
                }
                auto impl = std::dynamic_pointer_cast<PendingImpl<result_t>>(found->second);
                impl->data = std::make_shared<result_t>(p->data);
                impl->event.set();
            });
        }
        auto pending = std::make_shared<PendingImpl<result_t>>();
        auto id = curId;
        curId++;
        pendings.insert({id, pending});
        packet->emit<Request<T>>({id, t});
        mutex.unlock();
        pending->event.wait();
        mutex.lock();
        auto data = *pending->data;
        pendings.erase(id);
        mutex.unlock();
        return data;
    }

    template<class T>
    auto invoke(T& t) -> typename RpcTrait<T>::result_t {
        return invoke(std::move(t));
    }

    template<class T>
    void def(std::function<void(std::shared_ptr<T>, std::function<void(typename RpcTrait<T>::result_t)>)> cb) { //定义远程过程
        using result_t = typename RpcTrait<T>::result_t;
        packet->on<Request<T>>([=](auto r) {
            auto p = std::make_shared<T>(r->data);
            auto id = r->id;
            auto f = [this, id](result_t result){
                packet->emit<Response<T>>({id, result});
            };
            cb(p, f);
        });
    }

    template<class T>
    void def(std::function<typename RpcTrait<T>::result_t(std::shared_ptr<T>)> cb) {
        def<T>([=](auto p, auto r){
            r(cb(p));
        });
    }

private:
    std::shared_ptr<Packet> packet;
    std::set<std::size_t> registeredType = {};
    std::map<id_t, std::shared_ptr<Pending>> pendings;
    id_t curId = 0;
    rtthread::Mutex mutex = {kMutex};
    static const char* kMutex;
};

#include <utilities/singleton.hxx>
namespace Preset {
class Rpc: public Singleton<Rpc>, public ::Rpc {
    friend singleton_t;
    Rpc(): ::Rpc(Packet::get()) { }
};
}

