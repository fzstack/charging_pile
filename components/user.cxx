/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-01-01     imgcr       the first version
 */

#include "user.hxx"
#include <unordered_map>

void User::watch(std::shared_ptr<Rc522Base> rc522) {
    rc522->oCardId += [this](auto value) {
        if(!value) return;
        //onCardSwipe(*value);
        lastCard = *value;
    };
}

void User::watch(std::shared_ptr<Keyboard> keyboard) {
    keyboard->oValue += [this](auto value) {
        if(!value) return;
        switch(*value) {
        case Keyboard::Keys::Ret:
            currPort = 0;
            break;
        case Keyboard::Keys::Ok:
            onConfirm(currPort, lastCard);
            currPort = 0;
            break;
        default:
            auto sel = std::unordered_map<Keyboard::Keys, int> {
                {Keyboard::Keys::K0, 0},
                {Keyboard::Keys::K1, 1},
                {Keyboard::Keys::K2, 2},
                {Keyboard::Keys::K3, 3},
                {Keyboard::Keys::K4, 4},
                {Keyboard::Keys::K5, 5},
                {Keyboard::Keys::K6, 6},
                {Keyboard::Keys::K7, 7},
                {Keyboard::Keys::K8, 8},
                {Keyboard::Keys::K9, 9},
            };
            auto found = sel.find(*value);
            currPort *= 10;
            currPort += found->second;
            break;
        }
    };
}
