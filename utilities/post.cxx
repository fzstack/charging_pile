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
#include <rtthread.h>

using namespace std;

void Post::poll(PollType type) {
    //TODO:
    //if(没有注册) throw;
    //rt_event_recv(event.get(), Events::ConnackOk, RT_EVENT_FLAG_AND | RT_EVENT_FLAG_CLEAR, RT_WAITING_FOREVER, RT_NULL);

    auto guard = shared_ptr<void>(nullptr, [=](auto) {
        pollingThread = nullptr;
    });
    pollingThread = rt_thread_self();

    //TODO: 队列插队规则?
    do {
        auto front = queue.pop();
        front->invoke();
    } while(type == PollType::Forever);
    //for each bit

}

