#pragma once
#include <rtconfig.h>
#include "local_fal_persistent_storage.hxx"
#include "remote_fal_persistent_storage.hxx"

namespace Preset {
#ifdef UPPER_END
using PersistentStorage = LocalFalPersistentStorage;
#endif

#ifdef LOWER_END
using PersistentStorage = RemoteFalPersistentStorage;
#endif
}




