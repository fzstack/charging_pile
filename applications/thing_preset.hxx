/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-02-05     imgcr       the first version
 */
#pragma once

#include "local_thing.hxx"
#include "remote_thing.hxx"

namespace Preset {
#ifdef UPPER_END
using Thing = RemoteThing;
#endif

#ifdef LOWER_END
using Thing = LocalThing;
#endif
}


