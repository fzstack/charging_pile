/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-08-26     imgcr       the first version
 */

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
