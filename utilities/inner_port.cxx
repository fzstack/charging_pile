#include "inner_port.hxx"
#include "nat_port.hxx"
#include <config/bsp.hxx>

InnerPort::InnerPort(): Port(0) {

}

InnerPort::InnerPort(rt_uint8_t value): Port(value) {

}

InnerPort::operator NatPort() const {
    return NatPort(Config::Bsp::kPortNum - value); //9...0 -> 1...10
}

bool InnerPort::validate() {
    return 0 <= value && value < Config::Bsp::kPortNum;
}
