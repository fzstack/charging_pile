#ifndef LOWER_END
#error "this test is only for lower end"
#endif

#include <rtthread.h>
#include <components/local_state_store.hxx>
#include <utilities/mp.hxx>
#include <config/bsp.hxx>
#include <array>
#include <memory>
#include <utilities/cmd.hxx>

using namespace std;

#define LOG_TAG "test.local_ss"
#define LOG_LVL LOG_LVL_DBG
#include <ulog.h>

class TestLocalStateStore: public LocalStateStore {
    using LocalStateStore::LocalStateStore;
public:
    void setState(State::Value value) {
        state = value;
    }
};

std::array<std::shared_ptr<TestLocalStateStore>, Config::Bsp::kPortNum> stores;

void test_local_state_store(int argc, char** argv) {
    ASSERT_MIN_NARGS(3);
    auto idx = atoi(argv[1]);
    ASSERT_ARG(idx, 0 <= idx && idx < Config::Bsp::kPortNum);
    auto state = atoi(argv[2]);
    ASSERT_ARG(state, 1 <= state && state <= 5);
    stores[idx]->setState((State::Value)state);
}

int init_test_local_state_store() {
    for(auto i = 0; i < Config::Bsp::kPortNum; i++) {
        stores[i] = make_shared<TestLocalStateStore>(i);
    }

    return RT_EOK;
}

MSH_CMD_EXPORT(test_local_state_store, )
INIT_APP_EXPORT(init_test_local_state_store);
