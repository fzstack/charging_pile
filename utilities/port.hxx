#pragma once

#include <rtthread.h>
#include "serializable.hxx"

class Port: public Serializable {
protected:
    Port(rt_uint8_t value);
public:
    virtual void serialize(Serializer& ser) override final;
    virtual void deserialize(Deserializer& des) override final;
    rt_uint8_t get() const;
protected:
    rt_uint8_t value;
};


