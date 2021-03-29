#ifdef UPPER_END
#include <components/packet.hxx>
namespace Preset {
const int
    Packet::kThreadStack = 3072,
    Packet::kThreadPrio = 5,
    Packet::kThreadTick = 2;
}
#endif
