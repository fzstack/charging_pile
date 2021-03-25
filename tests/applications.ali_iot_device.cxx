#ifdef TEST_ALI_IOT_DEVICE
#include <applications/ali_iot_device.hxx>

#define LOG_TAG "test.ali_iot"
#define LOG_LVL LOG_LVL_DBG
#include <ulog.h>

using namespace std;
using namespace json_literals;


static int init_test_ali_iot_device() {
    auto air724 = Preset::Air724::get();
    air724->init();

    auto aliIot = Preset::AliIotDevice::get();
    try {
        aliIot->services["query"] += [=](const Json params) -> Json {
            rt_kprintf("\033[34mparams: %s\n\033[0m", to_string(params).c_str());
            aliIot->emit("ic_number", {
                {"port", 1},
                {"ic_number", "test"},
            });
            return {};
        };

        aliIot->services["control"] += [](const Json params) -> Json {
            rt_kprintf("\033[34mport: %d\n\033[0m", params["not_found"_i]);
            rt_kprintf("\033[34mtimer_id: %d\n\033[0m", params["timer_id"_i]);
            rt_kprintf("\033[34mminutes: %d\n\033[0m", params["minutes"_i]);
            return {
                {"state", 1},
            };
        };

        aliIot->properties["version"] += [](const Json value) {
            rt_kprintf("\033[34mversion new value: %s\n\033[0m", to_string(value).c_str());
        };

        aliIot->login("prototype", "a1SvHKhmFbS", "ecc2b3bcfa8311585bc8818acf7239ce");
    } catch(exception& e) {
        LOG_E("%s", e.what());
    }
    return RT_EOK;
}

INIT_APP_EXPORT(init_test_ali_iot_device);
#endif
