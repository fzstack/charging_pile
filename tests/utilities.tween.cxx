#include <rtthread.h>
#include <utilities/tween.hxx>
#include <utilities/cmd.hxx>

#define LOG_TAG "test.tween"
#define LOG_LVL LOG_LVL_DBG
#include <ulog.h>

class TestTween: public Tween<int> {
    using Tween<int>::Tween;
    virtual int update(int& initVal, int& targetVal, float pos) override {
        return initVal + (targetVal - initVal) * pos;
    }
};

TestTween tween(0, 1000, 10);

static void test_tween(int argc, char** argv) {
    ASSERT_MIN_NARGS(2);
    auto target = atoi(argv[1]);
    tween.setTargetVal(target);
}

static int init_test_tween() {
    tween.oCurrVal += [](auto value) {
        rt_kprintf("%d\n", value);
    };
    return RT_EOK;
}

MSH_CMD_EXPORT(test_tween, );
INIT_APP_EXPORT(init_test_tween);
