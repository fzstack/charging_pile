/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-12-25     imgcr       the first version
 */
#ifndef UTILITIES_EVENT_HXX_
#define UTILITIES_EVENT_HXX_

#include <rtthread.h>

//TODO: event pool
class Event {
public:
    Event();
    void set();
    void wait();
private:
    shared_ptr<rt_event> event = shared_ptr<rt_event>(rt_event_create("tt", RT_IPC_FLAG_FIFO), [](auto p) {
        rt_event_delete(p);
    });
};



#endif /* UTILITIES_EVENT_HXX_ */
