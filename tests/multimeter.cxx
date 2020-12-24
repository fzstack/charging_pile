/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-12-14     imgcr       the first version
 */

#include "multimeter.hxx"
#include <rtthread.h>
#include <utilities/cmd.hxx>
#include <applications/instances.hxx>

#define LOG_TAG "test.multimeter"
#define LOG_LVL LOG_LVL_DBG
#include <ulog.h>

#ifdef TEST_MULTIMETER

static std::shared_ptr<Multimeter::Channel> chA = {}, chB = {};

static void test_multimeter() {
    LOG_I("if cur and vol output is ok, then test passed");
}

static int init_test_multimeter() {
    auto multimeter = Instances::multimeter;

    chA = multimeter->getChannel(Multimeter::ChPort::A);
    chB = multimeter->getChannel(Multimeter::ChPort::B);

    chA->current += [](auto value) {
        if(value) {
            LOG_D("curA: %dmA", *value);
        }
    };

    chB->current += [](auto value) {
        if(value) {
            LOG_D("curB: %dmA", *value);
        }
    };

    chA->voltage += [](auto value) {
        if(value) {
            LOG_D("vol: %dV", *value);
        }
    };

    LOG_D("initing");
    multimeter->init();
    LOG_D("inited");

    return RT_EOK;
}

MSH_CMD_EXPORT(test_multimeter, );
INIT_APP_EXPORT(init_test_multimeter);
#endif



