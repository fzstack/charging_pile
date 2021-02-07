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
#include "ostream.hxx"
#include <memory>
#include <type_traits>
#include "mp.hxx"
#include <boost/pfr.hpp>
#include "serialize_utilities.hxx"

class Serializer {
public:
    Serializer(std::shared_ptr<OStream> ostream);

    template<class T>
    auto build(T&& t) -> std::enable_if_t<std::is_aggregate_v<std::decay_t<T>>> {
        boost::pfr::for_each_field(t, [&](auto& field) {
            build(field);
        });
    }

    template<class T>
    auto build(T&& t) -> std::enable_if_t<std::is_arithmetic_v<std::decay_t<T>> || std::is_enum_v<std::decay_t<T>>> {
        ostream->write(t);
    }

    template<class T>
    auto build(T&& t) -> std::enable_if_t<SerializeUtilities::is_cont_v<std::decay_t<T>>> {
        build<SerializeUtilities::prefix_len_t>(t.size());
        for(auto& c: t) {
            build(c);
        }
    }

    template<class T>
    auto build(T&& t) -> std::enable_if_t<SerializeUtilities::is_optional_v<std::decay_t<T>>>  {
        build(t.has_value());
        if(t.has_value()) {
            build(*t);
        }
    }

    template<class T>
    auto build(T&& t) -> std::enable_if_t<SerializeUtilities::is_map_v<std::decay_t<T>>>  {
        build<SerializeUtilities::prefix_len_t>(t.size());
        for(auto& [k, v]: t) {
            build(k);
            build(v);
        }
    }

private:
    std::shared_ptr<OStream> ostream;
};


