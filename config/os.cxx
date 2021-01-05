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
#include <components/state_store.hxx>
#include <applications/thing.hxx>
#include <applications/auto_poweroff_thing_deco.hxx>

using namespace Config;

const char
    *Os::kAliIotDeviceThreadName = "iot_dev",
    *LoadDetector::kTimer = "lodet",
    *VirtualLoadDetector::kMutex = "vlodet",
    *VirtualLoadDetector::kTimer = "vlodet",
    *Multimeter::kTimer = "mulmtr",
    *StateStore::kMutex = "ststor",
    *AutoPoweroffThingDeco::kTimer = "tapoff",
    *Thing::kMutex = "thing";

namespace Preset {
const rt_tick_t CloudTimer::kTime = 60000;
const char *CloudTimer::kName = "cloud";
}
