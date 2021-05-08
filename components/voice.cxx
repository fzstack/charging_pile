/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-04-09     imgcr       the first version
 */

#include "voice.hxx"
#include <cmath>

using namespace std;

Voice& Voice::amount(int cents) {
    auto yuan = cents / 100;
    integer(yuan);
    auto centRemained = cents % 100;
    if(centRemained != 0) {
        fragm(VoiceFragment::Dot);
        for(auto i = 1; i >= 0; i--) {
            auto currBit = (centRemained / int(pow(10, i))) % 10;
            if(currBit != 0 || i != 0) {
                fragm(numbers[currBit]);
            }
        }
    }
    fragm(VoiceFragment::Yuan);
    return *this;
}

Voice& Voice::integer(int number) {
    auto msb = msb10(number);
    auto meetZero = false;
    for(auto i = msb; i >= 0; i--) {
        auto currBit = (number / int(pow(10, i))) % 10;
        if(currBit != 0) {
            if(meetZero) {
                meetZero = false;
                fragm(numbers[0]);
            }
            if(msb != 1 || i != 1 || currBit != 1) {
                fragm(numbers[currBit]);
            }
            if(i != 0) {
                fragm(bits[i - 1]);
            }
        } else {
            meetZero = true;
        }
    }
    if(number == 0) {
        fragm(numbers[0]);
    }
    return *this;
}

Voice& Voice::port(const NatPort port) {
    return integer(port.get()).fragm(VoiceFragment::No).fragm(VoiceFragment::Port);
}

Voice& Voice::fragm(VoiceFragment fragm) {
    seq.push_back(fragm);
    return *this;
}

//如果为0则输出0、如果为1则输出0
int Voice::msb10(int n) {
    auto msb = 0;
    n /= 10;
    while(n != 0) {
        msb++;
        n /= 10;
    }
    return msb;
}

const std::array<VoiceFragment, 10> Voice::numbers = {
    VoiceFragment::Zero, VoiceFragment::One, VoiceFragment::Two, VoiceFragment::Three,
    VoiceFragment::Four, VoiceFragment::Five, VoiceFragment::Six, VoiceFragment::Seven,
    VoiceFragment::Eight, VoiceFragment::Nine,
};

const std::array<VoiceFragment, 4> Voice::bits = {
    VoiceFragment::Ten, VoiceFragment::Hundred, VoiceFragment::Thousand, VoiceFragment::TenThousand,
};
