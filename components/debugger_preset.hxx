#pragma once
#include "local_debugger.hxx"
#include "remote_debugger.hxx"

namespace Preset {
#ifdef UPPER_END
using Debugger = LocalDebugger;
#endif

#ifdef LOWER_END
using Debugger = RemoteDebugger;
#endif
}