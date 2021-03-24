/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-03-23     imgcr       the first version
 */

#ifdef TEST_USER

#include <components/user.hxx>
#include <rtthread.h>
#include <utilities/f.hxx>
#include <string>

using namespace std;

int init_test_user() {
    auto user = Preset::User::get();
    user->onConfirm += [](int port, string cardId) {
        F{} << "mock start @"_r << port << ", cardId"_r << cardId << endln;
    };

    F{} << "user test init ok, please test"_r << endln;
    return RT_EOK;
}

INIT_APP_EXPORT(init_test_user);

#endif
