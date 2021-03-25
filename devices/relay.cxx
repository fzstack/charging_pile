#include "relay.hxx"

using namespace std;

Relay::Relay(rt_base_t pin, Polar polar): pin(pin), polar(polar) {
    value.onChanged += [this](auto val) {
        if(val) {
            switch(*val) {
            case Relay::On:
                *this->pin = onPinVal;
                break;
            case Relay::Off:
                *this->pin = offPinVal;
                break;
            }
        } else {
            *this->pin = nullopt;
        }
    };
}
