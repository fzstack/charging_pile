/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-03-24     imgcr       the first version
 */

#include "port.hxx"
#include <utilities/serializer.hxx>
#include <utilities/deserializer.hxx>

Port::Port(rt_uint8_t value): value(value) {

}

void Port::serialize(Serializer& ser) {
    ser.build(value);
}

void Port::deserialize(Deserializer& des) {
    value = des.parse<rt_uint8_t>();
}

rt_uint8_t Port::get() {
    return value;
}
