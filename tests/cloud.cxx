/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-12-30     imgcr       the first version
 */
#include "cloud.hxx"
#include <rtthread.h>

#define LOG_TAG "test.cloud"
#define LOG_LVL LOG_LVL_DBG
#include <ulog.h>

using namespace std;

#ifdef TEST_CLOUD
static void test_cloud_init() {
    auto cloud = Preset::Cloud::get();
    cloud->onQuery += []() {
        rt_kprintf("\033[95mcloud on query\n\033[0m");
    };

    cloud->onControl += [](int port, int timerId, int minutes) {
        rt_kprintf("\033[95mport: %d\ntimer_id: %d\nminutes: %d\n\033[0m", port, timerId, minutes);
        return Cloud::ServiceResult::Succeed;
    };

    cloud->onStop += [](int port, int timerId) {
        rt_kprintf("\033[95mport: %d\ntimer_id: %d\n\033[0m", port, timerId);
        return Cloud::ServiceResult::Succeed;
    };

    cloud->init();
}

static void test_cloud_cur_data() {
    auto cloud = Preset::Cloud::get();
    cloud->setCurrentData({
        Cloud::CurrentData {
           port: 1,
           timerId: 1,
           leftMinutes: 1,
           state: Cloud::CurrentData::State::LoadNotReady,
           current: 0,
           voltage: 0,
           consumption: 0,
           fuse: Cloud::CurrentData::Fuse::Normal,
       },
       Cloud::CurrentData {
          port: 2,
          timerId: 2,
          leftMinutes: 1,
          state: Cloud::CurrentData::State::LoadNotReady,
          current: 0,
          voltage: 0,
          consumption: 0,
          fuse: Cloud::CurrentData::Fuse::Normal,
      },
    });
}

MSH_CMD_EXPORT(test_cloud_init, );
MSH_CMD_EXPORT(test_cloud_cur_data, );
#endif
