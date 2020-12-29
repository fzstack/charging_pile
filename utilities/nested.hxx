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
    using nested_t = Nested<Outer>;
    using outer_t = Outer;
    Nested(outer_t* outer): outer(outer) { }
protected:
    outer_t* outer;
};



#endif /* UTILITIES_NESTED_HXX_ */
