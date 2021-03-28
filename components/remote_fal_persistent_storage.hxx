#pragma once

#include <memory>
#include <config/co.hxx>
#include <components/rpc.hxx>
#include <co/remote.hxx>
#include <functional>

class RemoteFalPersistentStorage: public Remote {
public:
    RemoteFalPersistentStorage(std::shared_ptr<Rpc> rpc);

    template<class T>
    auto make() {
        return rpc->invoke<Rpcs::PersistentStorage::Make<T>>({});
    }

    template<class T>
    void make(std::function<void(std::shared_ptr<T>)> cb) {
        rpc->invoke<Rpcs::PersistentStorage::Make<T>>({}, [cb](auto p) {
            auto pdata = std::get_if<std::shared_ptr<T>>(&p);
            if(pdata == nullptr) return;
            cb(*pdata);
        });
    }

private:
    std::shared_ptr<Rpc> rpc;
};

#include <utilities/singleton.hxx>
namespace Preset {
class RemoteFalPersistentStorage: public Singleton<RemoteFalPersistentStorage>, public ::RemoteFalPersistentStorage {
    friend singleton_t;
    RemoteFalPersistentStorage(): ::RemoteFalPersistentStorage(Rpc::get()) { }
};
}
