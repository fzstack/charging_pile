/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-01-11     imgcr       the first version
 */
#ifdef TEST_PERSISTENT_STORAGE
#include <components/persistent_storage_preset.hxx>
#include <stdexcept>
#include <co/remote.hxx>
#include <type_traits>
#include <utilities/f.hxx>
#include <string>
#include <utilities/cmd.hxx>

#define LOG_TAG "test.persto"
#define LOG_LVL LOG_LVL_DBG
#include <ulog.h>

using namespace std;


struct TestConfigInt {
    int value = 233;
};

static void test_persistent_storage_int() {
    try {
        auto storage = Preset::PersistentStorage::get();
        auto test = storage->make<TestConfigInt>();
        F{} << "prev value: "_r << test->value << endln;
        test->value++;
    } catch (const exception& e) {
        LOG_E(e.what());
    }
}

static int init_test_persistent_storage_int() {
    auto storage = Preset::PersistentStorage::get();
#ifdef UPPER_END
    storage->def<TestConfigInt>();
#endif
    return RT_EOK;
}

INIT_APP_EXPORT(init_test_persistent_storage_int);
MSH_CMD_EXPORT(test_persistent_storage_int, );

struct TestConfigStr {
    string value = "hello";
};

static void test_persistent_storage_str(int argc, char** argv) {
    ASSERT_MIN_NARGS(2);
    try {
        auto storage = Preset::PersistentStorage::get();
        auto test = storage->make<TestConfigStr>();
        F{} << "prev value: "_r << test->value << endln;
        test->value = argv[1];
    } catch (const exception& e) {
        LOG_E(e.what());
    }
}

static int init_test_persistent_storage_str() {
    auto storage = Preset::PersistentStorage::get();
#ifdef UPPER_END
    storage->def<TestConfigStr>();
#endif
    return RT_EOK;
}

INIT_APP_EXPORT(init_test_persistent_storage_str);
MSH_CMD_EXPORT(test_persistent_storage_str, );


#endif


