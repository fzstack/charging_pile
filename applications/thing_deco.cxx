/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-01-03     imgcr       the first version
 */

#include "thing_deco.hxx"

ChargerInfo& ThingDeco::getInfo(int port) {
    return outer->infos[port];
}

std::shared_ptr<User> ThingDeco::getUser() {
    return outer->user;
}

std::shared_ptr<LastCharger> ThingDeco::getLast() {
    return outer->last;
}
