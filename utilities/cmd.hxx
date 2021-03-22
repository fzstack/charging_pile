/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-12-10     imgcr       the first version
 */
#ifndef UTILITIES_CMD_UTILITIES_HXX_
#define UTILITIES_CMD_UTILITIES_HXX_

#include <rtthread.h>

#define ASSERT_MIN_NARGS(n) \
    do { \
        if(argc < n) { \
            LOG_E("too few args"); \
            return; \
        } \
    } while(0)\

#define ASSERT_ARG(name, cond) \
    do { \
        if(!(cond)) { \
            LOG_E("bad " #name " arg"); \
            return; \
        } \
    } while(0) \

#endif /* APPLICATIONS2_CMD_UTILITIES_HXX_ */
