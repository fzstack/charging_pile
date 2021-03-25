#pragma once

#include "state_store_base.hxx"

class RemoteStateStore: public StateStoreBase {
public:
    RemoteStateStore(int idx);
};

#include <utilities/singleton.hxx>
namespace Preset {
template<int R>
class RemoteStateStore: public Singleton<RemoteStateStore<R>>, public ::RemoteStateStore {
    friend class Singleton<RemoteStateStore<R>>;
    RemoteStateStore(): ::RemoteStateStore(R) {}
};
}
