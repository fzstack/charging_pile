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

#include "local_state_store.hxx"
#include "remote_state_store.hxx"

namespace Preset {
#ifdef UPPER_END
template<int R>
using StateStore = RemoteStateStore<R>;
#endif

#ifdef LOWER_END
template<int R>
using StateStore = LocalStateStore<R>;
#endif
}


