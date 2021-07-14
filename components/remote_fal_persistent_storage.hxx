#pragma once

#include <memory>
#include <config/co.hxx>
#include <components/rpc.hxx>
#include <components/packet.hxx>
#include <co/remote.hxx>
#include <functional>
#include <Mutex.h>
#include <optional>
#include <utilities/tiny_type_id.hxx>
#include <utilities/err.hxx>

#define LOG_RFPS_NULL_PTR

class RemoteFalPersistentStorage: public Remote {
public:
    RemoteFalPersistentStorage(std::shared_ptr<Rpc> rpc, std::shared_ptr<Packet> packet);

//    template<class T>
//    auto make() {
//        return rpc->invoke<Rpcs::PersistentStorage::Make<T>>({});
//    }
//
//    template<class T>
//    void make(std::function<void(std::shared_ptr<T>)> cb) {
//        mutex.lock();
//        //clear up all released ptr
//        for(auto iter = buf.begin(); iter != buf.end();) {
//            if(iter->second.lock() == nullptr) {
//                iter = buf.erase(iter);
//            } else {
//                ++iter;
//            }
//        }
//        auto found = buf.find(TypeId<T>::get());
//        if(found != buf.end()) {
//            auto p = found->second.lock();
//            if(p != nullptr) {
//                mutex.unlock();
//                cb(std::reinterpret_pointer_cast<T>(p));
//                return;
//            }
//        }
//        mutex.unlock();
//        rpc->invoke<Rpcs::PersistentStorage::Make<T>>({}, [this, cb](auto p) {
//            auto pdata = std::get_if<std::shared_ptr<T>>(&p);
//            if(pdata == nullptr) {
////#ifdef LOG_RFPS_NULL_PTR
////                rt_kprintf("W: ps made a null ptr for %s\n", typeid(T).name());
////#endif
//                cb(nullptr);
//                return;
//            }
//            mutex.lock();
//            buf[TypeId<T>::get()] = *pdata;
//            mutex.unlock();
//            cb(*pdata);
//        });
//    }

    template<class T>
    void read(std::function<void(std::optional<T>)> cb) {
        rpc->invoke<Rpcs::PersistentStorage::Read<T>>({}, [this, cb](auto p) {
            auto pdata = std::get_if<T>(&p);
            if(pdata == nullptr) {
                cb(std::nullopt);
                return;
            }
            cb(*pdata);
        });
    }

    template<class T>
    T read() {
        throw not_implemented{"do not call this func"};
    }

    template<class T>
    void write(T&& t) {
        packet->emit<Rpcs::PersistentStorage::Write<T>>({t});
    }

    void reset() {
        // throw not_implemented{""};
    }

private:
    std::shared_ptr<Rpc> rpc;
    std::shared_ptr<Packet> packet;
    std::map<std::size_t, std::weak_ptr<void>> buf;
    rtthread::Mutex mutex = {"rfps"};
};

#include <utilities/singleton.hxx>
namespace Preset {
class RemoteFalPersistentStorage: public Singleton<RemoteFalPersistentStorage>, public ::RemoteFalPersistentStorage {
    friend singleton_t;
    RemoteFalPersistentStorage(): ::RemoteFalPersistentStorage(Rpc::get(), Packet::get()) { }
};
}
