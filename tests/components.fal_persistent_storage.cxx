/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-03-20     imgcr       the first version
 */

#ifdef TEST_FAL_PERSISTENT_STORAGE

#include <components/fal_persistent_storage.hxx>
#include <vector>
#include <string>

#include <utilities/f.hxx>
#include <utilities/cmd.hxx>

#define LOG_TAG "test.falps"
#define LOG_LVL LOG_LVL_DBG
#include <ulog.h>

using namespace std;

struct TestConf {
    int a = 5;
};

static void test_fal_ps(int argc, char** argv) {
    auto fps = Preset::FalPersistentStorage::get();
    auto test = fps->make<TestConf>();
    rt_kprintf("a = %d\n", test->a);
    test->a++;
}

struct TestVecConf {
    vector<int> values = {};
};

static void test_fal_ps_vec(int argc, char** argv) {
    auto fps = Preset::FalPersistentStorage::get();
    auto test = fps->make<TestVecConf>();
    F{} << "prev value: "_r << test->values << endln;
    if(test->values.size() == 0) {
        test->values.push_back(1);
    } else {
        test->values.push_back(test->values.back() + 1);
    }
}

struct TestStrConf {
    string value = "init_value";
};

static void test_fal_ps_str(int argc, char** argv) {
    ASSERT_MIN_NARGS(2);
    auto fps = Preset::FalPersistentStorage::get();
    auto test = fps->make<TestStrConf>();
    F{} << "prev value: "_r << test->value << endln;
    test->value = argv[1];
}



MSH_CMD_EXPORT(test_fal_ps, );
MSH_CMD_EXPORT(test_fal_ps_vec, );
MSH_CMD_EXPORT(test_fal_ps_str, );

#endif

