/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-01-20     imgcr       the first version
 */

#include "app.hxx"
#include "app_preset.hxx"
#include "thing.hxx"
#include "proxy_cloud.hxx"

using namespace std;

void runApp() {
#ifdef RUN_APP
    Preset::App::get()->run();
#endif
}

