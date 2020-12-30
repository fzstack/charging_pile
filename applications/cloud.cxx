/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-12-28     imgcr       the first version
 */

#include "cloud.hxx"

using namespace std;


Cloud::Cloud(shared_ptr<CloudTimer> timer): timer(timer) {

}

void Cloud::init() {
    timer->start();
}
