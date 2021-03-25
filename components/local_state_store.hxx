#pragma once

#include "state_store.hxx"

class LocalStateStore: public StateStore {
public:
    LocalStateStore(int idx);
};

#include <utilities/singleton.hxx>
namespace Preset {
template<int R>
class LocalStateStore: public Singleton<LocalStateStore<R>>, public ::LocalStateStore {
    friend class Singleton<LocalStateStore<R>>;
    LocalStateStore(): ::LocalStateStore(R) {}
};
}
