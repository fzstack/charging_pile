/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-12-13     imgcr       the first version
 */
#ifdef TEST_RGB_STATE_NOTIFIER
#include <components/rgb_state_notifier.hxx>
#include <components/state_store_base.hxx>

#include <rtthread.h>
#include <utilities/cmd.hxx>
#include <memory.h>
#include <map>
#include <functional>
#include <array>
#include <config/bsp.hxx>
#include <utilities/mp.hxx>

#define LOG_TAG "test.rgb_state_notifier"
#define LOG_LVL LOG_LVL_DBG
#include <ulog.h>

using namespace std;

class FakeStore: public StateStoreBase {
public:
    FakeStore() = default;
    void setState(State::Value state) {
        this->state = state;
    }
};

static array<std::shared_ptr<FakeStore>, Config::Bsp::kPortNum> states;

static void test_rgb_state_notifier(int argc, char** argv) {
    ASSERT_MIN_NARGS(3);

    auto store = atoi(argv[1]);
    ASSERT_ARG(store, 0 <= store && store < Config::Bsp::kPortNum);

    auto stateVal = atoi(argv[2]);
    ASSERT_ARG(state, 1 <= stateVal && stateVal <= 5);

    states[store]->setState(State::Value(stateVal));
}

static int init_test_rgb_state_notifier() {
    for(auto i = 0; i < Config::Bsp::kPortNum; i++) {
        magic_switch<Config::Bsp::kPortNum>{}([&](auto v){
            rt_uint32_t used;
            rt_memory_info(nullptr, &used, nullptr);
            auto before = used;
            auto notifier = Preset::RgbStateNotifier<decltype(v)::value>::get();
            rt_memory_info(nullptr, &used, nullptr);
            rt_kprintf("[%d] notifier inc: %d\n", i, used - before);
            before = used;
            states[i] = make_shared<FakeStore>();
            notifier->watch(states[i]);
            states[i]->setState(State::LoadNotInsert);
            rt_memory_info(nullptr, &used, nullptr);
            rt_kprintf("[%d] store inc: %d\n", i, used - before);
        }, i);
    }
    return RT_EOK;
}

MSH_CMD_EXPORT(test_rgb_state_notifier, );
INIT_APP_EXPORT(init_test_rgb_state_notifier);
#endif
