#pragma once

#include <memory>
#include <config/co.hxx>
#include <components/rpc.hxx>
#include <co/remote.hxx>

class RemoteFalPersistentStorage: public Remote {
public:
    RemoteFalPersistentStorage(std::shared_ptr<Rpc> rpc);

    template<class T>
    auto make() {
        return rpc->invoke<Rpcs::PersistentStorage::Make<T>>({});
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
