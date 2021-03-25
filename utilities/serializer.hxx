#pragma once
#include "ostream.hxx"
#include <memory>
#include <type_traits>
#include "mp.hxx"
#include <boost/pfr.hpp>
#include <set>
#include <map>
#include <functional>
#include "serialize_utilities.hxx"
#include "serializable.hxx"

class Serializer {
public:
    Serializer(std::shared_ptr<OStream> ostream, std::function<void(std::shared_ptr<void>, rt_uint8_t index)> holder = nullptr);

    template<class T>
    auto build(T&& t) -> std::enable_if_t<std::is_base_of_v<Serializable, std::decay_t<T>>> {
        t.serialize(*this);
    }

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
    auto build(T&& t) -> std::enable_if_t<SerUtilities::is_cont_v<std::decay_t<T>>> {
        build<SerUtilities::prefix_len_t>(t.size());
        for(auto& c: t) {
            build(c);
        }
    }

    template<class T>
    auto build(T&& t) -> std::enable_if_t<SerUtilities::is_optional_v<std::decay_t<T>>>  {
        build(t.has_value());
        if(t.has_value()) {
            build(*t);
        }
    }

    template<class T>
    auto build(T&& t) -> std::enable_if_t<SerUtilities::is_map_v<std::decay_t<T>>>  {
        build<SerUtilities::prefix_len_t>(t.size());
        for(auto& [k, v]: t) {
            build(k);
            build(v);
        }
    }

    template<class T>
    auto build(T&& t) -> std::enable_if_t<SerUtilities::is_array_v<std::decay_t<T>>>  {
        for(auto& c: t) {
            build(c);
        }
    }

    template<class T>
    auto build(T&& t) -> std::enable_if_t<SerUtilities::is_ptr_v<std::decay_t<T>>>  {
        //发送指针地址+数据值
        auto ptr = (size_t)t.get();
        auto found = storedPtr.find(ptr);
        if(found == storedPtr.end()) { //没找到
            auto idx = next++;
            storedPtr[ptr] = idx;
            if(holder) holder(t, idx);
            build(idx);
            build(*t);
        } else {
            build(found->second);
        }
    }

    template<class T>
    auto build(T&& t) -> std::enable_if_t<SerUtilities::is_weak_v<std::decay_t<T>>>  {
        build(t.lock());
    }

private:
    std::shared_ptr<OStream> ostream;
    std::function<void(std::shared_ptr<void>, rt_uint8_t index)> holder;
    std::map<std::size_t, rt_uint8_t> storedPtr = {{0, 0}};
    rt_uint8_t next = 1;
};


