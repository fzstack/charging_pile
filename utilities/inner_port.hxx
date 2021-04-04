#pragma once
#include <rtthread.h>
#include "port.hxx"

class NatPort;
class InnerPort: public Port {
public:
    InnerPort();
    explicit InnerPort(rt_uint8_t value);
    operator NatPort() const;
    bool validate();
};


