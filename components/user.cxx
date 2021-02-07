/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-01-01     imgcr       the first version
 */

#include "user.hxx"

void User::watch(std::shared_ptr<Rc522Base> rc522) {
    rc522->oCardId += [this](auto value) {
        if(!value) return;
        onCardSwipe(*value);
    };
}
