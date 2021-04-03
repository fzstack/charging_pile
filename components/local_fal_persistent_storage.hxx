#pragma once

#include "fal_persistent_storage.hxx"
#include <memory>
#include <utilities/f.hxx>
#include <components/packet.hxx>
#include <components/rpc.hxx>
#include <config/co.hxx>
#include <co/local.hxx>
#include <optional>

//代理模式
class LocalFalPersistentStorage: public Local {
public:
    LocalFalPersistentStorage(std::shared_ptr<FalPersistentStorage> storage, std::shared_ptr<Rpc> rpc, std::shared_ptr<Packet> packet);

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
        cb(read<T>());
    }

    template<class T>
    void write(T&& t) {
        storage->write(t);
    }

    template<class T>
    void def() { //请在预设类中调用，定义可远程访问的配置
//        rpc->def<Rpcs::PersistentStorage::Make<T>>([this](auto p) {
//#ifdef TEST_PERSISTENT_STORAGE
//            F{} << "remote cfg maked"_r << endln;
//#endif
//            return make<T>();
//        });

        rpc->def<Rpcs::PersistentStorage::Read<T>>([this](auto p) {
            return read<T>();
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
};

#include <utilities/singleton.hxx>
#include <applications/things/decos/backuper.hxx>
#include <applications/things/decos/params/current_limiter.hxx>
#include <applications/things/decos/params/data_setter.hxx>
#include <applications/things/decos/fuse_detecter.hxx>
#include <applications/things/decos/params/noload_detecter.hxx>
#include <utilities/mp.hxx>
#include <config/bsp.hxx>

namespace Preset {
class LocalFalPersistentStorage: public Singleton<LocalFalPersistentStorage>, public ::LocalFalPersistentStorage {
    friend singleton_t;
    LocalFalPersistentStorage(): ::LocalFalPersistentStorage(FalPersistentStorage::get(), Rpc::get(), Packet::get()) {
        using namespace Things::Decos;
//        for(auto i = 0; i < Config::Bsp::kPortNum; i++) {
//            magic_switch<Config::Bsp::kPortNum>{}([&](auto v){
//                def<Backuper::Backup<decltype(v)::value>>();
//            }, i);
//        }

        def<Params::CurrentLimiter>(); //0.64kB per config
        def<Params::DataSetter>();
        //def<FuseDetecter::Params>();
        def<Params::NoloadDetecter>();
    }
};
}

