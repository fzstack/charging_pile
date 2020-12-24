/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-12-23     imgcr       the first version
 */

#include "post.hxx"

void Post::poll() {
    //TODO:
    //if(没有注册) throw;
    //rt_event_recv(event.get(), Events::ConnackOk, RT_EVENT_FLAG_AND | RT_EVENT_FLAG_CLEAR, RT_WAITING_FOREVER, RT_NULL);
    auto recved = event_t{};
    rt_event_recv(event.get(), std::numeric_limits<event_t>::max(), RT_EVENT_FLAG_OR | RT_EVENT_FLAG_CLEAR, RT_WAITING_FOREVER, &recved);

    auto bits = std::bitset<std::numeric_limits<event_t>::digits>(recved);

    for(auto i = 0u; i < bits.size(); i++) {
        if(~bits[i]) continue;
        delivers[i]->invoke();
    }

    //for each bit

}

