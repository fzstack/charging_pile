/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-12-13     imgcr       the first version
 */

#include "rgb_state_notifier.hxx"
#include <rtthread.h>
#include <utilities/cmd.hxx>
#include <applications/instances.hxx>
#include <memory.h>
#include <map>
#include <functional>

#define LOG_TAG "test.rgb_state_notifier"
#define LOG_LVL LOG_LVL_DBG
#include <ulog.h>

using namespace std;

#ifdef TEST_RGB_STATE_NOTIFIER
static shared_ptr<FakeStore> fakeA, fakeB;

void test_rgb_state_notifier(int argc, char** argv) {
    ASSERT_MIN_NARGS(3);
    auto sel = std::map<std::string, shared_ptr<FakeStore>> {
        {"A", fakeA},
        {"B", fakeB},
    };

    auto found = sel.find(argv[1]);
    ASSERT_ARG(store, found != sel.end());

    auto stateVal = atoi(argv[2]);
    ASSERT_ARG(state, 1 <= stateVal && stateVal <= 5);

    found->second->state = State(stateVal);
}

static int init_test_rgb_state_notifier() {
    auto notifierA = Instances::rgbStateNotifierA;
    auto notifierB = Instances::rgbStateNotifierB;

    fakeA = make_shared<FakeStore>();
    fakeB = make_shared<FakeStore>();

    notifierA->watch(fakeA);
    notifierB->watch(fakeB);

    return RT_EOK;
}

MSH_CMD_EXPORT(test_rgb_state_notifier, );
INIT_APP_EXPORT(init_test_rgb_state_notifier);
#endif
