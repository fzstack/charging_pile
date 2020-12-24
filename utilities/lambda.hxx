/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-12-23     imgcr       the first version
 */
#ifndef UTILITIES_LAMBDA_HXX_
#define UTILITIES_LAMBDA_HXX_

namespace lambda_detail
{
    template<class Ret, class Cls, class IsMutable, class... Args>
    struct types
    {
        using is_mutable = IsMutable;

        enum { arity = sizeof...(Args) };

        using return_type = Ret;

        template<size_t i>
        struct arg
        {
            typedef typename std::tuple_element<i, std::tuple<Args...>>::type type;
        };
    };
}

template<class Ld>
struct lambda_type
    : lambda_type<decltype(&Ld::operator())>
{};

template<class Ret, class Cls, class... Args>
struct lambda_type<Ret(Cls::*)(Args...)>
    : lambda_detail::types<Ret,Cls,std::true_type,Args...>
{};

template<class Ret, class Cls, class... Args>
struct lambda_type<Ret(Cls::*)(Args...) const>
    : lambda_detail::types<Ret,Cls,std::false_type,Args...>
{};



#endif /* UTILITIES_LAMBDA_HXX_ */
