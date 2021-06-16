/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-03-31     imgcr       the first version
 */
#pragma once

namespace Things::Decos::Params {
    struct CurrentLimiter {
        int maxCurrentMiA = 2000; //电流变化阈值
    };
}

/*
{
    "current_limit": 2000,
    "upload_thr": 200,
    "fused_thr": 0,
    "noload_curr_thr": 20,
    "done_curr_thr": 88
}
*/
