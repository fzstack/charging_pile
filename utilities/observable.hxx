/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-08-24     imgcr       the first version
 */
#ifndef UTILITIES_OBSERVABLE_HXX_
#define UTILITIES_OBSERVABLE_HXX_

#include "signals.hxx"
#include "observer.hxx"

template<class T>
class Observer;

template <class T>
struct Observable {
    using type = T;
    friend class Observer<T>;
    Observable() { }
    Observable(T value): value(value) { }
    Observable(const Observable<T>& obj) = delete;

    T operator=(const T newVal) {
        if(value == newVal)
            return newVal;
        value = newVal;
        onChanged(newVal);

        return newVal;
    }

    bool operator==(const T& other) {
        return value == other;
    }

    T operator++(int) {
        auto old = value;
        value++;
        onChanged(value);
        return old;
    }

    operator T() const {
        return value;
    }

    T operator *() const {
        return value;
    }

    const T* operator->() {
        return &value;
    }

public:
    Signals<void(const T& value)> onChanged;
private:
    T value;
};



#endif /* APPLICATIONS2_OBSERVABLE_HXX_ */
