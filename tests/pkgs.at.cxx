#include <rtthread.h>
#include <rtdevice.h>
#include <at.h>

int init_test_at() {
    struct serial_configure conf = RT_SERIAL_CONFIG_DEFAULT;
    rt_base_t bufsz = 512;
    conf.bufsz = bufsz; //NOTE: buff不够大会导致接收AT数据不正常
    auto serial = rt_device_find("uart2");
    rt_device_control(serial, RT_DEVICE_CTRL_CONFIG, &conf);
    at_client_init("uart2", bufsz);
    return RT_EOK;
}

INIT_APP_EXPORT(init_test_at);
