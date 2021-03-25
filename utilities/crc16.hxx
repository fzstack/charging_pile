#ifndef UTILITIES_CRC16_HXX_
#define UTILITIES_CRC16_HXX_

#include <rtthread.h>
extern "C" {
#include "crc16.h"
}

class Crc16 {
public:
    void reset();
    void update(uint8_t dat);
    rt_uint16_t get();

    rt_uint16_t val = CRC16_INIT_VAL;
};



#endif /* UTILITIES_CRC16_HXX_ */
