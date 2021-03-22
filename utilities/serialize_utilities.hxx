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
#include <array>
#include <list>
#include <memory>

namespace SerUtilities {
using prefix_len_t = rt_uint16_t;

template<class T> struct is_cont: public std::false_type {};
template<class... Args> struct is_cont<std::vector<Args...>>: public std::true_type {};
template<class... Args> struct is_cont<std::list<Args...>>: public std::true_type {};
template<> struct is_cont<std::string>: public std::true_type {};
template<class T> inline constexpr bool is_cont_v = is_cont<T>::value;

template<class T> struct is_optional: public std::false_type {};
template<class... Args> struct is_optional<std::optional<Args...>>: public std::true_type {};
template<class T> inline constexpr bool is_optional_v = is_optional<T>::value;

template<class T> struct is_map: public std::false_type {};
template<class... Args> struct is_map<std::map<Args...>>: public std::true_type {};
template<class T> inline constexpr bool is_map_v = is_map<T>::value;

template<class T> struct is_vec: public std::false_type {};
template<class... Args> struct is_vec<std::vector<Args...>>: public std::true_type {};
template<class T> inline constexpr bool is_vec_v = is_vec<T>::value;

template<class T> struct is_list: public std::false_type {};
template<class... Args> struct is_list<std::list<Args...>>: public std::true_type {};
template<class T> inline constexpr bool is_list_v = is_list<T>::value;

template<class T> struct is_array: public std::false_type {};
template<class... Args> struct is_array<std::array<Args...>>: public std::true_type {};
template<class T> inline constexpr bool is_array_v = is_array<T>::value;

template<class T> struct is_ptr: public std::false_type {};
template<class... Args> struct is_ptr<std::shared_ptr<Args...>>: public std::true_type {};
template<class T> inline constexpr bool is_ptr_v = is_ptr<T>::value;

template<class T> struct is_weak: public std::false_type {};
template<class... Args> struct is_weak<std::weak_ptr<Args...>>: public std::true_type {};
template<class T> inline constexpr bool is_weak_v = is_weak<T>::value;

}



