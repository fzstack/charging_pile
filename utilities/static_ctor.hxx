/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-08-25     imgcr       the first version
 */
#pragma once

template <class T>
struct StaticCtor {
private:
    struct Helper {
        Helper() {
            T::staticCtor();
        }
    };

protected:
    StaticCtor() {
        static Helper helper;
    }
};
