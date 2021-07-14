#ifndef CDRIVERS_QUEUED_UART_HXX_
#define CDRIVERS_QUEUED_UART_HXX_

#include <rtthread.h>
#include <rtdevice.h>

class QueuedUart {
public:
    QueuedUart(const char* deviceName, struct serial_configure* conf = nullptr);

    template <class T>
    rt_size_t send(const T data) const {
        return send((void*)&data, sizeof(T));
    }

    rt_size_t send(void* data, int len) const;

    virtual int recv(void* data, int len, rt_int32_t timeout = 10);
    void clear();

private:
    rt_device_t device;
    rt_event_t event;
    volatile int rx_remain = 0;
    struct Events {
        enum Value {
            UartIndicate = 1,
        };
    };

    const char* deviceName;
};



#endif /* CDRIVERS_QUEUED_UART_HXX_ */
