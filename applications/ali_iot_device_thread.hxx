/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-12-25     imgcr       the first version
 */
#ifndef APPLICATIONS_ALI_IOT_DEVICE_THREAD_HXX_
#define APPLICATIONS_ALI_IOT_DEVICE_THREAD_HXX_

#include <Thread.h>
#include "ali_iot_device.hxx"
#include <utilities/nested.hxx>
#include <utilities/post.hxx>

class AliIotDevice;

class AliIotDeviceThread: public rtthread::Thread, public Nested<AliIotDevice> {
    using nested_t = Nested<AliIotDevice>;
    friend class AliIotDevice;
private:
    AliIotDeviceThread(outer_t* outer);

protected:
    virtual void run(void *p) override;

public:
    Post post = {};
};



#endif /* APPLICATIONS_ALI_IOT_DEVICE_THREAD_HXX_ */
