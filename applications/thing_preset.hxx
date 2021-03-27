#pragma once

#ifdef ENABLE_REMOTE
#include "local_thing.hxx"
#include "remote_thing.hxx"

namespace Preset {
#ifdef UPPER_END
using Thing = RemoteThing;
#endif

#ifdef LOWER_END
using Thing = LocalThing;
#endif
}

#else
#include "thing.hxx"
using Thing = ThingPre
#endif
