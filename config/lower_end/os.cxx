#ifdef LOWER_END
#include <components/packet.hxx>
namespace Preset {
const int
    Packet::kThreadStack = 2048,
    Packet::kThreadPrio = 5,
    Packet::kThreadTick = 2;
}
#endif
