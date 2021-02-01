/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-02-01     imgcr       the first version
 */

#ifdef TEST_LOADING_WIDGET
#include <ui/loading_widget.hxx>
#include <rtthread.h>

#define LOG_TAG "test.lod_wgt"
#define LOG_LVL LOG_LVL_DBG
#include <ulog.h>


static int init_test_loading_widget() {
    auto loading = Preset::LoadingWidget::get();
    rt_kprintf("init: %08x\n", loading.get());
    return RT_EOK;
}

INIT_APP_EXPORT(init_test_loading_widget);

#endif

