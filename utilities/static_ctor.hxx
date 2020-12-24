/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-08-25     imgcr       the first version
 */
#ifndef UTILITIES_STATIC_CTOR_HXX_
#define UTILITIES_STATIC_CTOR_HXX_

template <class T>
struct StaticCtor {
private:
    struct Helper {
        Helper() {
            T::staticCtor();
        }
    };

protected:
    StaticCtor() {
        //TODO: 待测试
        static Helper helper;
    }
};



#endif /* UTILITIES_STATIC_CTOR_HXX_ */
