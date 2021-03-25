#pragma once

#include <memory>

class Serializer;
class Deserializer;
class Serializable {
public:
    virtual void serialize(Serializer& ser) = 0;
    virtual void deserialize(Deserializer& des) = 0;
};


