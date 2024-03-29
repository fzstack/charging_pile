#pragma once

#include <rtthread.h>

class IStream {
public:
    template<class T>
    T read() {
        T t;
        readData((rt_uint8_t*)&t, sizeof(T));
        return t;
    }

    virtual int readData(rt_uint8_t* data, int len) = 0;
    virtual void seek(int pos) {}
    virtual bool isInvalid() {return false;}
};


