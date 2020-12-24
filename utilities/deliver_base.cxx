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

DeliverBase::DeliverBase(outer_t* outer, id_t id): base_t(outer), id(id) {

}

void DeliverBase::sendEvent() {
    rt_kprintf("result proc: emit event, outer: %08x, event: %08x\n", outer, outer->event.get());
    rt_event_send(outer->event.get(), 1 << id);
}

