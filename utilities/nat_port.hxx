#pragma once
#include <rtthread.h>
#include "port.hxx"
#include <config/bsp.hxx>

class InnerPort;
class NatPort: public Port {
public:
    NatPort();
    explicit NatPort(rt_uint8_t value);
    operator InnerPort() const;
    bool validate();
    static NatPort min() {
        return NatPort{1};
    }

    static NatPort max() {
        return NatPort{Config::Bsp::kPortNum};
    }

    void operator++() {
        value++;
    }

    bool operator<=(const NatPort& other) {
        return value <= other.value;
    }

    rt_uint8_t operator-(const NatPort& other) {
        return value - other.value;
    }
};


