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
    auto msb = msb10(yuan);
    auto meetZero = false;

    //13.00 十三元
    //23.00 二十三元
    //113.00 一百一十三元
    //1010.00 一千零一十三元
    //1001.00 一千零一元
    //0.00 零元
    //13.50 十三点五元
    //0.05 零点零五元

    for(auto i = msb; i >= 0; i--) {
        auto currBit = (yuan / int(pow(10, i))) % 10;
        if(currBit != 0) {
            if(meetZero) {
                meetZero = false;
                seq.push_back(numbers[0]);
            }
            if(msb != 1 || i != 1 || currBit != 1) {
                seq.push_back(numbers[currBit]);
            }
            if(i != 0) {
                seq.push_back(bits[i - 1]);
            }
        } else {
            meetZero = true;
        }
    }

    if(yuan == 0) {
        seq.push_back(numbers[0]);
    }

    auto centRemained = cents % 100;
    if(centRemained != 0) {
        seq.push_back(VoiceFragment::Dot);
        for(auto i = 1; i >= 0; i--) {
            auto currBit = (centRemained / int(pow(10, i))) % 10;
            if(currBit != 0 || i != 0) {
                seq.push_back(numbers[currBit]);
            }
        }
    }

    seq.push_back(VoiceFragment::Yuan);
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
