#ifndef CONFIG_OS_HXX_
#define CONFIG_OS_HXX_

#include <rtthread.h>

namespace Config {
class Os {
public:
    static constexpr int
        kThreadDefaultStackSize = 4096,
        kAliIotDeviceThreadPrio = 8,
        kAliIotDeviceThreadStackSize = 4096;

    static const char
        *kAliIotDeviceThreadName;
};
}

enum class Priority {
    High,
    Middle,
};

template<Priority Proi>
struct PriorityTrait {
    //static constexpr rt_uint8_t value = 0;
};

template<>
struct PriorityTrait<Priority::High> {
    static constexpr rt_uint8_t value = 2;
};

#ifdef UPPER_END
template<>
struct PriorityTrait<Priority::Middle> {
    static constexpr rt_uint8_t value = 8;
};
#endif

#ifdef LOWER_END
template<>
struct PriorityTrait<Priority::Middle> {
    static constexpr rt_uint8_t value = 15;
};
#endif

#endif /* CONFIG_OS_HXX_ */
