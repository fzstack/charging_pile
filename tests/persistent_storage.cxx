/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-01-11     imgcr       the first version
 */

#include "persistent_storage.hxx"
#include <stdexcept>

#define LOG_TAG "test.persto"
#define LOG_LVL LOG_LVL_DBG
#include <ulog.h>

using namespace std;

#ifdef TEST_PERSISTENT_STORAGE
static void test_persistent_storage_test() {
    try {
        auto storage = Preset::PersistentStorage::get();
        storage->test();
    } catch (const exception& e) {
        LOG_E(e.what());
    }
}

static void test_persistent_storage_format() {
    try {
        auto storage = Preset::PersistentStorage::get();
        storage->format();
    } catch (const exception& e) {
        LOG_E(e.what());
    }
}

static void test_persistent_storage_type_a() {
    struct Test {
        int value = 233; //默认值
    };

    try {
        auto storage = Preset::PersistentStorage::get();
        auto test = storage->make<Test>();
        test->value++;
        rt_kprintf("value: %d\n", test->value);
    } catch (const exception& e) {
        LOG_E(e.what());
    }
}

static void test_persistent_storage_type_b() {
    struct Test {
        int value = 789; //默认值
    };

    try {
        auto storage = Preset::PersistentStorage::get();
        auto test = storage->make<Test>();
        test->value--;
        rt_kprintf("value: %d\n", test->value);
    } catch (const exception& e) {
        LOG_E(e.what());
    }
}

MSH_CMD_EXPORT(test_persistent_storage_test, );
MSH_CMD_EXPORT(test_persistent_storage_format, );
MSH_CMD_EXPORT(test_persistent_storage_type_a, );
MSH_CMD_EXPORT(test_persistent_storage_type_b, );
#endif

