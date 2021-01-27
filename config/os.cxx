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
#include <applications/things/decos/current_limiter.hxx>
#include <applications/things/decos/counter.hxx>
#include <applications/things/decos/backuper.hxx>
#include <applications/things/decos/consumption_measurer.hxx>
#include <applications/things/decos/fuse_detecter.hxx>
#include <applications/things/decos/noload_detecter.hxx>
#include <components/persistent_storage.hxx>
#include <components/packet.hxx>

using namespace Config;
using namespace Things::Decos;

const char
    *Os::kAliIotDeviceThreadName = "iot_dev",
    *LoadDetector::kTimer = "lodet",
    *VirtualLoadDetector::kMutex = "vlodet",
    *VirtualLoadDetector::kTimer = "vlodet",
    *Multimeter::kTimer = "mulmtr",
    *StateStore::kMutex = "ststor",
    *Counter::kTimer = "tapoff",
    *::Thing::kMutex = "thing",
    *CurrentLimiter::kTimer = "kltdec",
    *CurrentLimiter::kMutex = "kltdec",
    *Backuper::kTimer = "btdec",
    *ConsumptionMeasurer::kTimer = "cmtdec",
    *ConsumptionMeasurer::kMutex = "cmtdec",
    *FuseDetecter::kTimer = "fdtdec",
    *NoloadDetecter::kTimer = "nltdec",
    *PersistentStorage::kMutex = "persto";

namespace Preset {
const rt_tick_t CloudTimer::kTime = 60000;
const char *CloudTimer::kName = "cloud";

const char* Packet::kThread = "packet";
const int
    Packet::kThreadStack = 1024,
    Packet::kThreadPrio = 20,
    Packet::kThreadTick = 2;
}
