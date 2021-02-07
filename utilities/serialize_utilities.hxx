/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-02-07     imgcr       the first version
 */
#pragma once
#include <rtthread.h>
#include <vector>
#include <string>
#include <optional>
#include <map>

namespace SerializeUtilities {
using prefix_len_t = rt_uint16_t;

template<class T> struct is_cont: public std::false_type {};
template<class T> inline constexpr bool is_cont_v = is_cont<T>::value;
template<class T> using is_cont_t = typename is_cont<T>::type;

template<class T>
struct is_cont<std::vector<T>>: public std::true_type {};

template<>
struct is_cont<std::string>: public std::true_type {};

template<class T> struct is_optional: public std::false_type {};
template<class T> inline constexpr bool is_optional_v = is_optional<T>::value;
template<class T> using is_optional_t = typename is_optional<T>::type;

template<class T>
struct is_optional<std::optional<T>>: public std::true_type {
    using type = std::decay_t<T>;
};

template<class T> struct is_map: public std::false_type {};
template<class T> inline constexpr bool is_map_v = is_map<T>::value;

template<class... Args>
struct is_map<std::map<Args...>>: public std::true_type {};

}



