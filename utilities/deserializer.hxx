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

#include "istream.hxx"
#include <memory>
#include <type_traits>
#include <boost/pfr.hpp>
#include "serialize_utilities.hxx"

class Deserializer {
public:
    Deserializer(std::shared_ptr<IStream> istream);

    template<class T>
    auto parse() -> std::enable_if_t<std::is_aggregate_v<std::decay_t<T>>, T> {
        T t;
        boost::pfr::for_each_field(t, [&](auto& field) {
            field = parse<std::decay_t<decltype(field)>>();
        });
        return t;
    }

    template<class T>
    auto parse() -> std::enable_if_t<std::is_arithmetic_v<std::decay_t<T>> || std::is_enum_v<std::decay_t<T>>, T> {
        T t = istream->read<T>();
        return t;
    }

    template<class T>
    auto parse() -> std::enable_if_t<SerializeUtilities::is_cont_v<std::decay_t<T>>, T> {
        auto size = parse<SerializeUtilities::prefix_len_t>();
        auto t = T(size, SerializeUtilities::is_cont_t<std::decay_t<T>>{});
        for(auto& c: t) {
            c = parse<std::decay_t<decltype(c)>>();
        }
        return t;
    }

    template<class T>
    auto parse() -> std::enable_if_t<SerializeUtilities::is_optional_v<std::decay_t<T>>, T> {
        auto t = T{};
        auto hasValue = parse<bool>();
        if(hasValue) {
            t = parse<SerializeUtilities::is_optional_t<std::decay_t<T>>>();
        }
        return t;
    }
    
    template<class T>
    auto parse() -> std::enable_if_t<SerializeUtilities::is_map_v<std::decay_t<T>>, T> {
        auto t = T{};
        auto size = parse<SerializeUtilities::prefix_len_t>();
        for(auto i = 0u; i < size; i++) {
            auto key = parse<typename T::key_type>();
            t[key] = parse<typename T::mapped_type>();
        }
        return t;
    }

private:
    std::shared_ptr<IStream> istream;
};


