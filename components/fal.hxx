/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-04-14     imgcr       the first version
 */
#pragma once

class Fal {
public:
    Fal();
};

#include <utilities/singleton.hxx>
namespace Preset {
class Fal: public Singleton<Fal>, public ::Fal {
    friend singleton_t;
    Fal(): ::Fal() {}
};
}


