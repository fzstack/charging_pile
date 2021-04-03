#pragma once
#include <rtthread.h>
#include "port.hxx"

class InnerPort;
class NatPort: public Port {
public:
    NatPort();
    explicit NatPort(rt_uint8_t value);
    operator InnerPort() const;
};


