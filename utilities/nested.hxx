/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-08-25     imgcr       the first version
 */
#ifndef UTILITIES_NESTED_HXX_
#define UTILITIES_NESTED_HXX_

template <class Outer>
struct Nested {
    using outer_t = Outer;
    Nested(Outer* outer): outer(outer) { }
protected:
    Outer* outer;
};



#endif /* UTILITIES_NESTED_HXX_ */
