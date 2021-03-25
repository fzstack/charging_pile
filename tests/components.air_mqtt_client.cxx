#ifdef TEST_AIR_MQTT_CLIENT
#include <devices/air724.hxx>
#include <components/air_components.hxx>
#include <utilities/err.hxx>

#define LOG_TAG "test.air_mqtt"
#define LOG_LVL LOG_LVL_DBG
#include <ulog.h>

using namespace std;

static void test_air_mqtt_client() {
    try {
        auto air724 = Preset::Air724::get();

        LOG_D("try make mqtt1");
        auto mqtt = air724->make<MqttClient>();

        mqtt->onConnected += [=]{
            LOG_D("connected");
        };

        mqtt->login("a1tltf2GJUn.iot-as-mqtt.cn-shanghai.aliyuncs.com", "863701042917152", "LzClcw5EEHU9vUHL7TSL000000", "^1^1608310653632^ec7d1f2c5463897");

    } catch(const exception& e) {
        LOG_E("error occured: %s", e.what());
    }
}

static int init_test_air_mqtt_client() {
    auto air724 = Preset::Air724::get();
    air724->init();
    return RT_EOK;
}

MSH_CMD_EXPORT(test_air_mqtt_client, );
INIT_APP_EXPORT(init_test_air_mqtt_client);
#endif
