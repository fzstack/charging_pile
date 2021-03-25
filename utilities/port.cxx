#include "port.hxx"
#include <utilities/serializer.hxx>
#include <utilities/deserializer.hxx>

Port::Port(rt_uint8_t value): value(value) {

}

void Port::serialize(Serializer& ser) {
    ser.build(value);
}

void Port::deserialize(Deserializer& des) {
    value = des.parse<rt_uint8_t>();
}

rt_uint8_t Port::get() const {
    return value;
}
