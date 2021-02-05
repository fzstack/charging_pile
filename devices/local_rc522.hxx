/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-02-05     imgcr       the first version
 */
#pragma once
#include "rc522.hxx"
#include <components/packet.hxx>
#include <memory>

class LocalRc522: public Rc522 {
public:
    LocalRc522(const char* spiBus, const char* spiDev, rt_base_t ssPin, std::shared_ptr<Packet> packet);
private:
    std::shared_ptr<Packet> packet;
};

#include <utilities/singleton.hxx>
namespace Preset {
class LocalRc522: public Singleton<LocalRc522>, public ::LocalRc522 {
    friend singleton_t;
    LocalRc522(): ::LocalRc522(kSpiBus, kSpiDev, kSsPin, Packet::get()) {
        init();
    }
    static const char *kSpiBus, *kSpiDev;
    static const rt_base_t kSsPin;
};
}
