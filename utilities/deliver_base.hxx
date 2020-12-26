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
#include <memory>

class Post;

class DeliverBase: private Nested<Post>, public std::enable_shared_from_this<DeliverBase> {
    using base_t = Nested<Post>;
protected:
    DeliverBase(outer_t* outer);

    void enqueue();
    rt_thread_t getPollingThread();
public:
    virtual void invoke() = 0;
};



#endif /* UTILITIES_DELIVER_BASE_HXX_ */
