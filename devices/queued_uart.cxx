#include "queued_uart.hxx"
#include <rthw.h>
#include <utilities/err.hxx>
#include <string>
#include <memory>

using namespace std;
using namespace string_literals;

QueuedUart::QueuedUart(const char* deviceName, struct serial_configure* conf): deviceName(deviceName) {
    event = rt_event_create("QU", RT_IPC_FLAG_FIFO);
    device = rt_device_find(deviceName);
    device->user_data = this;
    if(conf != nullptr)
        rt_device_control(device, RT_DEVICE_CTRL_CONFIG, conf);
    rt_device_open(device, RT_DEVICE_FLAG_INT_RX);
    rt_device_set_rx_indicate(device, [](auto dev, auto size) -> rt_err_t {
        auto self = (QueuedUart*)dev->user_data;
        rt_event_send(self->event, Events::UartIndicate);
        self->rx_remain = size;
        return RT_EOK;
    });
}

rt_size_t QueuedUart::send(void* data, int len) const {
    return rt_device_write(device, 0, data, len);
}

int QueuedUart::recv(void* data, int len, rt_int32_t timeout) {
    rt_err_t retval;
    while(rx_remain < len) {
        if((retval = rt_event_recv(event, Events::UartIndicate, RT_EVENT_FLAG_AND | RT_EVENT_FLAG_CLEAR, timeout, RT_NULL)) != RT_EOK) {
#ifdef __cpp_exceptions
            throw timeout_error{"timeout recv data from "s + deviceName};
#endif
            return -1;
        }
    }
    rt_base_t level = rt_hw_interrupt_disable();
    rx_remain -= len;
    rt_hw_interrupt_enable(level);
    rt_device_read(device, 0, data, len);
    return 0;
}

void QueuedUart::clear() {
    auto len = rx_remain;
    rt_base_t level = rt_hw_interrupt_disable();
    rx_remain = 0;
    rt_hw_interrupt_enable(level);

    auto data = shared_ptr<char[]>(new char[len]);
    rt_device_read(device, 0, data.get(), len);
}

