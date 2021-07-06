#include <devices/load_detector.hxx>
#include <utilities/cmd.hxx>
#include <utilities/mp.hxx>
#include <rtthread.h>
#include <functional>
#include <utilities/nat_port.hxx>
#include <utilities/inner_port.hxx>

#define LOG_TAG "test.load_det"
#define LOG_LVL LOG_LVL_DBG
#include <ulog.h>

using namespace std;
using namespace placeholders;

static int init_test_load_detector() {

    for(rt_uint8_t i = 0; i < Config::Bsp::kPortNum; i++) {
        magic_switch<Config::Bsp::kPortNum>{}([&](auto v) {
            auto lodDet = Preset::LoadDetector<decltype(v)::value>::get();
            lodDet->oState += [i](auto state){
                if(state) {
                    auto port = NatPort{InnerPort{i}};
                    LOG_D("[%d] %s", port.get(), *state ? "pluged" : "unpluged");
                }
            };

        }, i);
    }
    LOG_I("inited, plug to test");
    return RT_EOK;
}

INIT_APP_EXPORT(init_test_load_detector);
