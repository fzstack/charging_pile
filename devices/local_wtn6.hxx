/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-02-04     imgcr       the first version
 */
#pragma once

#include "wtn6.hxx"
#include <components/packet.hxx>
#include <config/co.hxx>

class LocalWtn6: public Wtn6 {
public:
    LocalWtn6(rt_base_t dataPin, rt_base_t busyPin, std::shared_ptr<Packet> packet);

private:
    std::shared_ptr<Packet> packet;
};

#include <utilities/singleton.hxx>
namespace Preset {
class LocalWtn6: public Singleton<LocalWtn6>, public ::LocalWtn6 {
    friend singleton_t;
    LocalWtn6(): ::LocalWtn6(kDataPin, kBusyPin, Packet::get()) {}
    static const rt_base_t kDataPin, kBusyPin;
};
}
