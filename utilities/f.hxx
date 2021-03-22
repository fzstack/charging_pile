/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-03-22     imgcr       the first version
 */
#pragma once

#include <rtthread.h>
#include <type_traits>
#include "serialize_utilities.hxx"
#include <boost/pfr.hpp>
#include "mp.hxx"
#include <set>
//#include <cxxabi.h>
#include <string.h>

template<class T>
class Row {
public:
    Row(T t): t(t) {

    }

    const T& get() {
        return t;
    }

private:
    const T t;
};

class F {
public:

    template<class T>
    auto operator <<(T&& t) -> std::enable_if_t<std::is_same_v<std::decay_t<T>, Row<char*>>, F&> {
        rt_kprintf(t.get());
        return *this;
    }

    template<class T>
    auto operator <<(T&& t) -> std::enable_if_t<std::is_same_v<std::decay_t<T>, Row<char>>, F&> {
        rt_kprintf("%c", t.get());
        return *this;
    }

    template<class T>
    auto operator <<(T&& t) -> std::enable_if_t<!std::is_same_v<std::decay_t<T>, Row<char*>> && !std::is_same_v<std::decay_t<T>, Row<char>>, F&> {
        print(t);
        return *this;
    }

    template<class T>
    auto println(T&& t) {
        print(t);
        rt_kprintf("\n");
    }

    template<class T>
    auto print(T&& t) -> std::enable_if_t<std::is_aggregate_v<std::decay_t<T>>> {
        //int status;
        //auto realname = abi::__cxa_demangle(typeid(t).name(), 0, 0, &status);
//        auto last = strrchr(realname, ':');
//        if(last != nullptr) {
//            realname = last + 1;
//        }
        rt_kprintf("\033[34m%s\033[0m {", typeid(t).name());
        auto size = boost::pfr::tuple_size_v<std::decay_t<T>>;
        auto i = 0u;
        boost::pfr::for_each_field(t, [&](auto& field) {
            print(field);
            if(i + 1 < size) {
                rt_kprintf(", ");
            }
            i++;
        });
        rt_kprintf("}");
    }

    template<class T>
    auto print(T&& t) ->  std::enable_if_t<std::is_integral_v<std::decay_t<T>>> {
        rt_kprintf("\033[33m%d\033[0m", t);
    }

    template<class T>
    auto cont_sn(T&& t) -> std::enable_if_t<SerUtilities::is_vec_v<std::decay_t<T>>, const char*> {
        return "vec";
    }

    template<class T>
    auto cont_sn(T&& t) -> std::enable_if_t<SerUtilities::is_list_v<std::decay_t<T>>, const char*> {
        return "list";
    }

    template<class T>
    auto print(T&& t) -> std::enable_if_t<SerUtilities::is_cont_v<std::decay_t<T>> && !std::is_same_v<std::string, std::decay_t<T>>> {
        rt_kprintf("\033[34m%s\033[0m {", cont_sn(t));
        auto i = 0u;
        for(auto& c: t) {
            print(c);
            if(i + 1 < t.size()) {
                rt_kprintf(", ");
            }
            i++;
        }
        rt_kprintf("}");
    }

    template<class T>
    auto print(T&& t) -> std::enable_if_t<std::is_same_v<std::string, std::decay_t<T>>> {
        rt_kprintf("\033[32m\"%s\"\033[0m", t.c_str());
    }

    template<class T>
    auto print(T&& t) -> std::enable_if_t<std::is_same_v<char*, std::decay_t<T>>> {
        rt_kprintf("\033[32m\"%s\"\033[0m", t);
    }

    template<class T>
    auto print(T&& t) -> std::enable_if_t<SerUtilities::is_ptr_v<std::decay_t<T>>>  {
        if(t == nullptr) {
            rt_kprintf("[\033[35mnull\033[0m]");
            return;
        }
        auto ptr = (std::size_t)t.get();
        rt_kprintf("[\033[35m%08x\033[0m ", ptr);
        if(storedPtr.count(ptr)) {
            rt_kprintf("..");
        } else {
            storedPtr.insert(ptr);
            print(*t);
        }
        rt_kprintf("]");
    }

    template<class T>
    auto print(T&& t) -> std::enable_if_t<SerUtilities::is_weak_v<std::decay_t<T>>>  {
        print(t.lock());
    }


    std::set<std::size_t> storedPtr;
};


//template<>
//Fmt& Fmt::operator <<(Row<char*>&& t) {
//    rt_kprintf(t.get());
//    return *this;
//}
//
//template<>
//Fmt& Fmt::operator <<(Row<char>&& t) {
//    rt_kprintf("%c", t.get());
//    return *this;
//}

extern Row<char> endln;

static inline Row<char*> operator "" _r(const char* t, std::size_t) {
    return {(char*)t};
}
