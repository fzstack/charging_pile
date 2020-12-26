/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-12-24     imgcr       the first version
 */

#include "deliver_base.hxx"
#include "post.hxx"
#include <rtthread.h>
#include <stdexcept>

using namespace std;

DeliverBase::DeliverBase(outer_t* outer): base_t(outer) {

}

rt_thread_t DeliverBase::getPollingThread() {
    return outer->pollingThread;
}

void DeliverBase::enqueue() {
    try {
        rt_kprintf("deliver try share from this\n");
        outer->queue.push(shared_from_this());
    } catch(const exception& e) {
        rt_kprintf("error: %s\n", e.what());
    }

}

