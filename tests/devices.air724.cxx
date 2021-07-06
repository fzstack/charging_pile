#include <utilities/cmd.hxx>
#include <rtthread.h>
#include <devices/air724.hxx>
#include <components/air_components.hxx>
#include <exception>

#define LOG_TAG "test.air724"
#define LOG_LVL LOG_LVL_DBG
#include <ulog.h>

using namespace std;


static void test_air724() {
    auto air724 = Preset::Air724::get();
    try {
        auto ess = air724->make<AirEssential>();

        auto csq = ess->getCsq();
        auto imei = ess->getImei();
        auto iccid = ess->getIccid();

        LOG_I("CSQ: %d", csq);
        LOG_I("imei: %s", imei.c_str());
        LOG_I("iccid: %s", iccid.c_str());
    } catch(exception& e) {
        LOG_E("%s", e.what());
    }
}

static void test_air724_reset() {
    Preset::Air724::get()->reset();
}

static int test_air724_init() {
    auto air724 = Preset::Air724::get();
    try {
        air724->init();
        LOG_D("inited");
    } catch(exception& e) {
        LOG_E("init failed, %s", e.what());
    }
    return RT_EOK;
}

MSH_CMD_EXPORT(test_air724, );
MSH_CMD_EXPORT(test_air724_reset, );
MSH_CMD_EXPORT(test_air724_init,);
