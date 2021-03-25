#pragma once

#include <limits>
#include <algorithm>

template <class T>
float normalize(T t) {
    return (float(t) - std::numeric_limits<T>::min()) / (float(std::numeric_limits<T>::max()) - std::numeric_limits<T>::min());
}

template<typename T>
T saturate(T val, T min, T max) {
    return std::min(std::max(val, min), max);
}
