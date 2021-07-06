/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-04-08     imgcr       the first version
 */
#pragma once
#include <rtconfig.h>
#ifdef UPPER_END
#include "rebooter_local.hxx"
namespace Preset {
using Rebooter = RebooterLocal;
}
#endif

#ifdef LOWER_END
#include "rebooter_remote.hxx"
namespace Preset {
using Rebooter = RebooterRemote;
}
#endif


