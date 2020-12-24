/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-12-17     imgcr       the first version
 */
#ifndef UTILITIES_MP_HXX_
#define UTILITIES_MP_HXX_

#include <tuple>
#include <type_traits>
#include <functional>

template <template<class> class Cond, class Tuple>
struct find_type;

//所有元素均不满足条件
template <template<class> class Cond>
struct find_type<Cond, std::tuple<>> {
    using type = std::void_t<>;
};

template <template<class> class Cond, class Tuple>
using find_type_t = typename find_type<Cond, Tuple>::type;

//找到了满足条件的元素
template <template<class> class Cond, class T, class... Ts>
struct find_type<Cond, std::tuple<T, Ts...>>: std::conditional<Cond<T>::value, T, find_type_t<Cond, std::tuple<Ts...>>> {};


#endif /* UTILITIES_MP_HXX_ */
