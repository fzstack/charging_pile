/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-08-26     imgcr       the first version
 */
#ifndef DEVICES_WTN6_HXX_
#define DEVICES_WTN6_HXX_

#include <rtthread.h>
#include "wtn6_base.hxx"

class Wtn6: public Wtn6Base {
public:
    Wtn6(rt_base_t dataPin, rt_base_t busyPin);

//    bool isBusy();
    virtual void write(rt_uint8_t data) override;


private:
    void writeBit(bool bit);

private:
    const rt_base_t dataPin, busyPin;
};


#endif /* DEVICES_WTN6_HXX_ */
