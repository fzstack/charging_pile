/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-01-03     imgcr       the first version
 */

#include "base.hxx"

using namespace rtthread;
using namespace Things::Decos;

ChargerInfo& Base::getInfo(int port) {
    return outer->infos[port];
}

std::shared_ptr<User> Base::getUser() {
    return outer->user;
}

std::shared_ptr<LastCharger> Base::getLast() {
    return outer->last;
}

rtthread::Lock Base::getLock() {
    return Lock(outer->mutex);
}
