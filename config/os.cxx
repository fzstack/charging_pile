/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-12-26     imgcr       the first version
 */

#include "os.hxx"
#include <applications/cloud_timer.hxx>
#include <components/virtual_load_detector.hxx>
#include <devices/load_detector.hxx>
#include <components/multimeter.hxx>

using namespace Config;

const char
    *Os::kAliIotDeviceThreadName = "iot_dev";

const char *LoadDetector::kTimer = "lodet";
const char *VirtualLoadDetector::kMutex = "vlodet";
const char *VirtualLoadDetector::kTimer = "vlodet";
const char *Multimeter::kTimer = "mulmtr";

namespace Preset {
const rt_tick_t CloudTimer::kTime = 60000;
const char *CloudTimer::kName = "cloud";
}
