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

#include <memory>

class Serializer;
class Deserializer;
class Serializable {
public:
    virtual void serialize(Serializer& ser) = 0;
    virtual void deserialize(Deserializer& des) = 0;
};


