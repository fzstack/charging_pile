#include "inner_port.hxx"
#include "nat_port.hxx"

InnerPort::InnerPort(): Port(0) {

}

InnerPort::InnerPort(rt_uint8_t value): Port(value) {

}

InnerPort::operator NatPort() {
    return NatPort(value + 1);
}
