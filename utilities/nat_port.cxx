#include "nat_port.hxx"
#include "inner_port.hxx"
#include <config/bsp.hxx>

NatPort::NatPort(): Port(1) {

}

NatPort::NatPort(rt_uint8_t value): Port(value) {

}

NatPort::operator InnerPort() const {
    return InnerPort(Config::Bsp::kPortNum - value); // 1...10 -> 9...0
}

bool NatPort::validate() {
    return InnerPort{*this}.validate();
}

