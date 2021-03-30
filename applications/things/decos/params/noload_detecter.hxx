/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-03-30     imgcr       the first version
 */
#pragma once

namespace Things::Decos::Params {
    struct NoloadDetecter {
        int noloadCurrThr = 10; //空载电流阈值
    };
}

