#pragma once

#include "fal_persistent_storage.hxx"
#include <memory>
#include <utilities/f.hxx>
#include <components/packet.hxx>
#include <components/rpc.hxx>
#include <config/co.hxx>
#include <co/local.hxx>
#include <optional>
#include <utilities/shared_thread.hxx>

//代理模式
class LocalFalPersistentStorage: public Local {
public:
    LocalFalPersistentStorage(std::shared_ptr<FalPersistentStorage> storage, std::shared_ptr<Rpc> rpc, std::shared_ptr<Packet> packet, std::shared_ptr<SharedThread> thread);

//    template<class T>
//    auto make() {
//        return storage->make<T>();
//    }
//
//    template<class T>
//    auto make(std::function<void(std::shared_ptr<T>)> cb) {
//        cb(storage->make<T>());
//    }

    template<class T>
    auto read() {
        return storage->read<T>();
    }

    template<class T>
    void read(std::function<void(std::optional<T>)> cb) {
        thread->exec([this, cb]{
            cb(read<T>());
        });
    }

    template<class T>
    void write(T&& t) {
        T x = t;
        thread->exec([this, x]{
            storage->write<T>((T&&)std::move(x));
        });
    }

    template<class T>
    void def() { //请在预设类中调用，定义可远程访问的配置
        rpc->def<Rpcs::PersistentStorage::Read<T>>([this](auto p, auto r) {
            r(read<T>());
        });

        packet->on<Rpcs::PersistentStorage::Write<T>>([this](auto p) {
            write<T>(std::move(p->data));
        });
    }

    void reset() {
        storage->reset();
    }

private:
    std::shared_ptr<FalPersistentStorage> storage;
    std::shared_ptr<Rpc> rpc;
    std::shared_ptr<Packet> packet;
    std::shared_ptr<SharedThread> thread;
};

#include <utilities/singleton.hxx>

namespace Preset {
class LocalFalPersistentStorage: public Singleton<LocalFalPersistentStorage>, public ::LocalFalPersistentStorage {
    friend singleton_t;
    LocalFalPersistentStorage(): ::LocalFalPersistentStorage(FalPersistentStorage::get(), Rpc::get(), Packet::get(), SharedThread<Priority::Middle>::get()) {

    }
};
}

