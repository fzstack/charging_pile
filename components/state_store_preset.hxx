#pragma once

#include "local_state_store.hxx"
#include "remote_state_store.hxx"

namespace Preset {
#ifdef UPPER_END
template<int R>
using StateStore = RemoteStateStore<R>;
#endif

#ifdef LOWER_END
template<int R>
using StateStore = LocalStateStore<R>;
#endif
}


