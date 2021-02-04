/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-02-04     imgcr       the first version
 */
#pragma once

#include "local_wtn6.hxx"
#include "remote_wtn6.hxx"

namespace Preset {
#ifdef UPPER_END
using Wtn6 = LocalWtn6;
#endif

#ifdef LOWER_END
using Wtn6 = RemoteWtn6;
#endif
}
