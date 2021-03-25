#include "crc16.hxx"

void Crc16::reset() {
    val = CRC16_INIT_VAL;
}

void Crc16::update(uint8_t dat) {
    val = crc16_cyc_cal(val, &dat, sizeof(dat));
}

rt_uint16_t Crc16::get() {
    return val ^ CRC16_INIT_VAL;
}
