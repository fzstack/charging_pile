#ifdef TEST_VIRTUAL_LOAD_DETECTOR
#include <components/virtual_load_detector.hxx>
#include <components/multimeter.hxx>
#include <rtthread.h>
#include <config/bsp.hxx>
#include <utilities/mp.hxx>
#include <utilities/inner_port.hxx>
#include <utilities/nat_port.hxx>
#include <utilities/f.hxx>

#define LOG_TAG "test.vld"
#define LOG_LVL LOG_LVL_DBG
#include <ulog.h>

int test_virual_load_detector_init() {
    for(rt_uint8_t i = 0; i < Config::Bsp::kPortNum; i++) {
        auto port = NatPort{InnerPort{i}};
        magic_switch<Config::Bsp::kPortNum>{}([&](auto v) {
            auto vlodet = Preset::VirtualLoadDetector<decltype(v)::value>::get();
            vlodet->oState += [=](auto value) {
                if(!value) return;
                F{} << "vlod"_r << port.get() << " "_r << (*value ? "pluged"_r : "unpluged"_r) << endln;
            };
        }, i);
    }
    F{} << "test for vlodet init ok"_r << endln;
    return RT_EOK;
}

INIT_APP_EXPORT(test_virual_load_detector_init);
#endif
