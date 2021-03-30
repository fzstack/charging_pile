#pragma once

#include "fal_persistent_storage.hxx"
#include <memory>
#include <utilities/f.hxx>
#include <components/rpc.hxx>
#include <config/co.hxx>
#include <co/local.hxx>

//代理模式
class LocalFalPersistentStorage: public Local {
public:
    LocalFalPersistentStorage(std::shared_ptr<FalPersistentStorage> storage, std::shared_ptr<Rpc> rpc);

    template<class T>
    auto make() {
        return storage->make<T>();
    }

    template<class T>
    auto make(std::function<void(std::shared_ptr<T>)> cb) {
        cb(storage->make<T>());
    }

    template<class T>
    void def() { //请在预设类中调用，定义可远程访问的配置
        rpc->def<Rpcs::PersistentStorage::Make<T>>([this](auto p) {
#ifdef TEST_PERSISTENT_STORAGE
            F{} << "remote cfg maked"_r << endln;
#endif
            return make<T>();
        });
    }

    void reset() {
        storage->reset();
    }

private:
    std::shared_ptr<FalPersistentStorage> storage;
    std::shared_ptr<Rpc> rpc;
};

#include <utilities/singleton.hxx>
#include <applications/things/decos/backuper.hxx>
#include <applications/things/decos/params/current_limiter.hxx>
#include <applications/things/decos/data_setter.hxx>
#include <applications/things/decos/fuse_detecter.hxx>
#include <applications/things/decos/params/noload_detecter.hxx>
#include <utilities/mp.hxx>
#include <config/bsp.hxx>

namespace Preset {
class LocalFalPersistentStorage: public Singleton<LocalFalPersistentStorage>, public ::LocalFalPersistentStorage {
    friend singleton_t;
    LocalFalPersistentStorage(): ::LocalFalPersistentStorage(FalPersistentStorage::get(), Rpc::get()) {
        using namespace Things::Decos;
        for(auto i = 0; i < Config::Bsp::kPortNum; i++) {
            magic_switch<Config::Bsp::kPortNum>{}([&](auto v){
                def<Backuper::Backup<decltype(v)::value>>();
            }, i);
        }

        def<Params::CurrentLimiter>();
        def<DataSetter::Params>();
        def<FuseDetecter::Params>();
        def<Params::NoloadDetecter>();
    }
};
}

