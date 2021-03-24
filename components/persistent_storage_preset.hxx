/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-03-21     imgcr       the first version
 */
#pragma once

#include "local_fal_persistent_storage.hxx"
#include "remote_fal_persistent_storage.hxx"

namespace Preset {
#ifdef UPPER_END
using PersistentStorage = LocalFalPersistentStorage;
#endif

#ifdef LOWER_END
using PersistentStorage = RemoteFalPersistentStorage;
#endif
}




