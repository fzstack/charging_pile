/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-02-05     imgcr       the first version
 */

#include "local_rc522.hxx"
#include <config/co.hxx>

using namespace std;
using namespace Packets;

LocalRc522::LocalRc522(const char* spiBus, const char* spiDev, rt_base_t ssPin, shared_ptr<Packet> packet): Rc522(spiBus, spiDev, ssPin), packet(packet) {
    oCardId += [packet](auto cardId) {
        auto p = Devices::Rc522{nullopt};
        if(cardId) {
            auto tmp = Devices::Rc522::Value{};
            strncpy(tmp.value, cardId->c_str(), sizeof(tmp.value));
            p = {tmp};
        }
        packet->emit(p);
    };
}


