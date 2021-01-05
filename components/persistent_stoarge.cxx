/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-01-04     imgcr       the first version
 */

#include "persistent_storage.hxx"

PersistentStorage::PersistentStorage(at24cxx_device_t device): device(device) {

}


void PersistentStorage::format() {

}
