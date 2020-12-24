/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-12-24     imgcr       the first version
 */
#ifndef UTILITIES_DELIVER_BASE_HXX_
#define UTILITIES_DELIVER_BASE_HXX_

#include "nested.hxx"
#include <rtthread.h>

class Post;

class DeliverBase: private Nested<Post> {
    using base_t = Nested<Post>;
protected:
    using id_t = rt_uint32_t;
protected:
    DeliverBase(outer_t* outer, id_t id);

    void sendEvent();
public:
    virtual void invoke() = 0;

    id_t id;
};



#endif /* UTILITIES_DELIVER_BASE_HXX_ */
