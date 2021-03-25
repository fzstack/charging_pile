#pragma once

#include <rtthread.h>

class Wtn6Base {
public:
    virtual void write(rt_uint8_t data) = 0;

    template<class T>
    void write(T data) {
        write(rt_uint8_t(data));
    }
};


