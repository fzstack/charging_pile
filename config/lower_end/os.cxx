#ifdef LOWER_END
#include <components/packet.hxx>
#include <utilities/shared_thread.hxx>
namespace Preset {
const int
    Packet::kThreadStack = 2048,
    Packet::kThreadPrio = 5,
    Packet::kThreadTick = 2;

template<> const int SharedThread<Priority::Middle>::kThreadStack = 2048;
template<> const int SharedThread<Priority::Middle>::kThreadTick = 10;
}

#endif
