#include "inner_port.hxx"
#include "nat_port.hxx"
#include <config/bsp.hxx>

InnerPort::InnerPort(): Port(0) {

}

InnerPort::InnerPort(rt_uint8_t value): Port(value) {

}

InnerPort::operator NatPort() const {
    return NatPort(value + 1);
}

bool InnerPort::validate() {
    return 0 <= value && value < Config::Bsp::kPortNum;
}
