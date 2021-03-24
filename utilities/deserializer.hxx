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
#include "serializable.hxx"

class Deserializer {
public:
    Deserializer(std::shared_ptr<IStream> istream);

    template<class T>
    auto parse() -> std::enable_if_t<std::is_base_of_v<Serializable, std::decay_t<T>>, T> {
        auto t = T();
        t.deserialize(*this);
        return t;
    }

    template<class T>
    auto parse() -> std::enable_if_t<std::is_aggregate_v<T>, T> {
        T t;
        boost::pfr::for_each_field(t, [&](auto& field) {
            field = parse<std::remove_reference_t<decltype(field)>>();
        });
        return t;
    }

    template<class T>
    auto parse() -> std::enable_if_t<std::is_arithmetic_v<T> || std::is_enum_v<T>, T> {
        return istream->read<T>();
    }

    template<class T>
    auto parse() -> std::enable_if_t<SerUtilities::is_cont_v<T>, T> {
        auto size = parse<SerUtilities::prefix_len_t>();
        auto t = T(size, typename T::value_type{});
        for(auto& c: t) {
            c = parse<typename T::value_type>();
        }
        return t;
    }

    template<class T>
    auto parse() -> std::enable_if_t<SerUtilities::is_optional_v<T>, T> {
        auto t = T{};
        auto hasValue = parse<bool>();
        if(hasValue) {
            t = parse<typename T::value_type>();
        }
        return t;
    }
    
    template<class T>
    auto parse() -> std::enable_if_t<SerUtilities::is_map_v<T>, T> {
        auto t = T{};
        auto size = parse<SerUtilities::prefix_len_t>();
        for(auto i = 0u; i < size; i++) {
            auto key = parse<typename T::key_type>();
            t[key] = parse<typename T::mapped_type>();
        }
        return t;
    }

    template<class T>
    auto parse() -> std::enable_if_t<SerUtilities::is_array_v<T>, T> {
        auto t = T();
        for(auto& c: t) {
            c = parse<typename T::value_type>();
        }
        return t;
    }

    template<class T>
    auto parse() -> std::enable_if_t<SerUtilities::is_ptr_v<T> || SerUtilities::is_weak_v<T>, T> {
        auto idx = parse<rt_uint8_t>();

        if(idx == 0) {
            return std::shared_ptr<typename T::element_type>(nullptr);
        }
        auto found = ptrs.find(idx);
        if(found != ptrs.end()) {
            return std::reinterpret_pointer_cast<typename T::element_type>(found->second);
        }
        auto t = std::make_shared<typename T::element_type>();
        ptrs[idx] = t;
        *t = parse<typename T::element_type>();
        return t;
    }

private:
    std::shared_ptr<IStream> istream;
    std::map<rt_uint8_t, std::shared_ptr<void>> ptrs;
};


