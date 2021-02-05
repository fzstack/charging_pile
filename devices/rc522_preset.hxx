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

#include "local_rc522.hxx"
#include "remote_rc522.hxx"

namespace Preset {
#ifdef UPPER_END
using Rc522 = LocalRc522;
#endif

#ifdef LOWER_END
using Rc522 = RemoteRc522;
#endif
}


