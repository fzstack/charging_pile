/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-12-25     imgcr       the first version
 */

#include "ali_iot_device_thread.hxx"
#include <config/os.hxx>

using namespace Config;

AliIotDeviceThread::AliIotDeviceThread(outer_t* outer):
  Thread(Os::kAliIotDeviceThreadStackSize, Os::kAliIotDeviceThreadPrio, 20, Os::kAliIotDeviceThreadName), nested_t(outer) {

}

void AliIotDeviceThread::run(void *p) {
    post.poll();
}
