#pragma once

#include "state_store_base.hxx"
#include <utilities/static_ctor.hxx>

class RemoteStateStore: public StateStoreBase, public StaticCtor<RemoteStateStore> {
    friend static_ctor_t;
public:
    RemoteStateStore(int idx);
private:
    void on(State::Value value);
    static void staticCtor();
};

#include <utilities/singleton.hxx>
namespace Preset {
template<int R>
class RemoteStateStore: public Singleton<RemoteStateStore<R>>, public ::RemoteStateStore {
    friend class Singleton<RemoteStateStore<R>>;
    RemoteStateStore(): ::RemoteStateStore(R) {}
};
}
