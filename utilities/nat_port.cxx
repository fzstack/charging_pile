#include "nat_port.hxx"
#include "inner_port.hxx"

NatPort::NatPort(): Port(1) {

}

NatPort::NatPort(rt_uint8_t value): Port(value) {

}

NatPort::operator InnerPort() const {
    return InnerPort(value - 1);
}

bool NatPort::validate() {
    return InnerPort{*this}.validate();
}

