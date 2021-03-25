#include "nat_port.hxx"
#include "inner_port.hxx"

NatPort::NatPort(): Port(1) {

}

NatPort::NatPort(rt_uint8_t value): Port(value) {

}

NatPort::operator InnerPort() {
    return InnerPort(value - 1);
}

