#pragma once
#include <rtdevice.h>
#include <board.h>

struct pin_index
{
    int index;
    GPIO_TypeDef *gpio;
    uint32_t pin;
};
extern const struct pin_index pins[];


