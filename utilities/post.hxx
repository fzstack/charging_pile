/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-12-23     imgcr       the first version
 */
#ifndef UTILITIES_POST_HXX_
#define UTILITIES_POST_HXX_

#include <rtthread.h>
#include <memory>
#include <functional>
#include <type_traits>
#include <limits>
#include <bitset>
#include <map>
#include "err.hxx"
#include "nested.hxx"
#include "lambda.hxx"
#include "signals.hxx"
#include "deliver.hxx"

template<class T> struct Signals;

class DeliverBase;

class Post {
    template<class T> friend struct Signals;
    using id_t = rt_uint32_t;
    using event_t = rt_uint32_t; //WARNING: NO NOT MODIFY
    template <class L, class F> friend class Deliver;
    friend class DeliverBase;
public:
    Post(): event(std::shared_ptr<rt_event>(rt_event_create("post", RT_IPC_FLAG_FIFO), [](auto p) {
        rt_event_delete(p);
    })) {
        throw not_implemented{"ctor not impl"};
    }
    void poll();

    template<class F>
    auto& operator()(F&& f) {

        if(nextId >= std::numeric_limits<event_t>::digits)
            throw std::out_of_range{"deliver id is out of range"};

        auto deliver = std::make_shared<Deliver<F>>(this, std::forward<F>(f), nextId);
        delivers[nextId] = deliver;
        nextId++;

        return *deliver;
    }

private:
    std::shared_ptr<rt_event> event;
    std::map<id_t, std::shared_ptr<DeliverBase>> delivers = {};
    id_t nextId = 0;
};

#endif /* UTILITIES_POST_HXX_ */
