/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-08-25     imgcr       the first version
 */
#ifndef UTILITIES_OBSERVER_HXX_
#define UTILITIES_OBSERVER_HXX_

#include "signals.hxx"
#include "observable.hxx"
#include <functional>
template <class T>
struct Observable;

template<class T>
class Observer {
public:
    Observer(Observable<T>& obs): obs(obs) { }

    void operator+=(std::function<void(const T& value)> cb) {
        obs.onChanged += cb;
    }

    T value() const {
        return obs;
    }

    T operator*() const {
        return obs;
    }

public:
    Observable<T>& obs;
};



#endif /* UTILITIES_OBSERVER_HXX_ */
