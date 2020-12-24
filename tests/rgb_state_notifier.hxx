/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-12-13     imgcr       the first version
 */
#ifndef TESTS_RGB_STATE_NOTIFIER_HXX_
#define TESTS_RGB_STATE_NOTIFIER_HXX_

#include <components/rgb_state_notifier.hxx>
#include <components/state_store_base.hxx>

class FakeStore: public StateStoreBase {
    friend void test_rgb_state_notifier(int argc, char** argv);
public:
    FakeStore() = default;
};

#endif /* TESTS_RGB_STATE_NOTIFIER_HXX_ */
