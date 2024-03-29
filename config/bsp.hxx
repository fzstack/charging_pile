#ifndef CONFIG_BSP_HXX_
#define CONFIG_BSP_HXX_
#include <type_traits>

namespace Config {
class Bsp {
public:
    static constexpr int kPortNum = 10;

    template<class T, int R>
    using assert_t = std::enable_if_t<R >= 0 && R < kPortNum, T>;
};
}



#endif /* CONFIG_BSP_HXX_ */
