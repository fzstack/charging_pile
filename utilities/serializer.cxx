/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-02-07     imgcr       the first version
 */

#include "serializer.hxx"

using namespace std;

Serializer::Serializer(shared_ptr<OStream> ostream, function<void(shared_ptr<void>, rt_uint8_t index)> holder): ostream(ostream), holder(holder) {

}

