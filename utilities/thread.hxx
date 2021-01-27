/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-01-27     imgcr       the first version
 */
#ifndef UTILITIES_THREAD_HXX_
#define UTILITIES_THREAD_HXX_

#include <Thread.h>
#include <utilities/signals.hxx>

class Thread: public rtthread::Thread {
public:
    using rtthread::Thread::Thread;
protected:
    virtual void run(void *p) override;
public:
    Signals<void()> onRun;
};



#endif /* UTILITIES_THREAD_HXX_ */
