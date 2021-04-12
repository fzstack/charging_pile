#include <rtdevice.h>
#include <exception>
#include <stdexcept>
#include "air724.hxx"
#include <components/air_components.hxx>

#define LOG_TAG "dev.air724"
#define LOG_LVL LOG_LVL_DBG
#include <ulog.h>

using namespace std;

class AirEssential;

Air724::Air724(const char* atUartDevice, rt_base_t resetPin): resetPin(resetPin) {
    struct serial_configure conf = RT_SERIAL_CONFIG_DEFAULT;
    conf.bufsz = kAtBuffSize; //NOTE: buff不够大会导致接收AT数据不正常
    auto serial = rt_device_find(atUartDevice);
    rt_device_control(serial, RT_DEVICE_CTRL_CONFIG, &conf);
    at_client_init(atUartDevice, kAtBuffSize);
    atClient = at_client_get(atUartDevice);
    atClient->device->user_data = this;

    inited.onChanged += [this](auto value) {
        if(!value) return;
        if(at_client_obj_wait_connect(atClient, kTimeoutMs) != RT_EOK) {
            rt_kprintf("try init failed\n");
            rt_thread_mdelay(10000);
            if(at_client_obj_wait_connect(atClient, kTimeoutMs) != RT_EOK) {
                throw runtime_error{"connect timeout"};
            }
        }

        auto ess = make<AirEssential>();
        rt_kprintf("ess maked\n");
        ess->closeEcho();
        rt_kprintf("echo closed\n");
        ess->attachGprs();
        rt_kprintf("gprs attached\n");
        ess->activatePdp();
        rt_kprintf("pdpactivated\n");
    };
}

void Air724::reset(rt_base_t resetMs) {
    *resetPin = true;
    rt_thread_mdelay(resetMs);
    *resetPin = false;
    inited = false; //TODO: 由谁调用init函数?
}

void Air724::init() {
    inited = true;
}

