/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-03-24     imgcr       the first version
 */
#pragma once

#include <rtthread.h>
#include "serializable.hxx"

class Port: public Serializable {
protected:
    Port(rt_uint8_t value);
public:
    virtual void serialize(Serializer& ser) override final;
    virtual void deserialize(Deserializer& des) override final;
    rt_uint8_t get();
protected:
    rt_uint8_t value;
};


