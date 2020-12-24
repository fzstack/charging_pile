/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-12-25     imgcr       the first version
 */
#ifndef UTILITIES_QUEUE_HXX_
#define UTILITIES_QUEUE_HXX_

#include <queue>
#include <Mutex.h>
#include <Lock.h>
#include "event.hxx"

template <class T>
class Queue {
public:
    Queue() { }
    void push(T t) {
        auto lock = rtthread::Lock(m);
        q.push(t);
        e.set();
    }

    T pop() {
        auto lock = rtthread::Lock(m);
        while(q.empty()) {
            m.unlock();
            e.wait();
            m.lock();
        }
        T result = q.front();
        q.pop();
        return result;
    }

private:
    std::queue<T> q = {};
    rtthread::Mutex m = {};
    Event e = {};
};

#endif /* UTILITIES_QUEUE_HXX_ */
