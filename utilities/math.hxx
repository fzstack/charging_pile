#pragma once

#include <limits>
#include <algorithm>
#include <type_traits>
#include <cmath>

template <class T>
float normalize(T t) {
    return (float(t) - std::numeric_limits<T>::min()) / (float(std::numeric_limits<T>::max()) - std::numeric_limits<T>::min());
}

template<typename T>
T saturate(T val, T min, T max) {
    return std::min(std::max(val, min), max);
}

template<typename T>
auto is_same_val(T a, T b, T epsilon = 0.001) ->std::enable_if_t<std::is_floating_point_v<T>, bool> {
    return std::fabs(a - b) < epsilon;
}

template<class T>
T between(T& a, T& b, float pos) {
    return a + (b - a) * pos;
}
