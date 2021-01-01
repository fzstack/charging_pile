/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-12-29     imgcr       the first version
 */
#ifndef UTILITIES_SINGLETON_HXX_
#define UTILITIES_SINGLETON_HXX_

#include <memory>

template<class T>
class Singleton {
public:
    using singleton_t = Singleton<T>;
    template<class... A>
    static std::shared_ptr<T> get(A&&... a) {
        static auto inst = std::shared_ptr<T>(new T(std::forward<A>(a)...));
        return inst;
    }
};



#endif /* UTILITIES_SINGLETON_HXX_ */
