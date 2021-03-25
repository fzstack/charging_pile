#pragma once

#include <rtthread.h>

class OStream {
public:
    template<class T>
    void write(T&& t) {
        writeData((rt_uint8_t*)(void*)&t, sizeof(T));
    }

    virtual void writeData(rt_uint8_t* data, int len) = 0;
};

