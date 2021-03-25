#ifdef TEST_MULTIMETER
#include <components/multimeter.hxx>
#include <rtthread.h>
#include <utilities/cmd.hxx>
#include <config/bsp.hxx>
#include <utilities/mp.hxx>

#define LOG_TAG "test.multimeter"
#define LOG_LVL LOG_LVL_DBG
#include <ulog.h>

static void test_multimeter() {
    LOG_I("if cur and vol output is ok, then test passed");
}

static int init_test_multimeter() {
    for(auto i = 0; i < Config::Bsp::kPortNum; i++) {
        magic_switch<Config::Bsp::kPortNum>{}([&](auto v){
            auto chn = Preset::MultiMeterChannel<decltype(v)::value>::get();
            chn->current += [i](auto value) {
                if(!value) return;
                rt_kprintf("[%d] %dmA\n", i, *value);
            };
        }, i);
    }
    return RT_EOK;
}

MSH_CMD_EXPORT(test_multimeter, );
INIT_APP_EXPORT(init_test_multimeter);
#endif



