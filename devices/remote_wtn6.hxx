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

#include "wtn6_base.hxx"
#include <components/packet.hxx>

class RemoteWtn6: public Wtn6Base {
public:
    RemoteWtn6(std::shared_ptr<Packet> packet);
    virtual void write(rt_uint8_t data) override;

private:
    std::shared_ptr<Packet> packet;
};

#include <utilities/singleton.hxx>
namespace Preset {
class RemoteWtn6: public Singleton<RemoteWtn6>, public ::RemoteWtn6 {
    friend singleton_t;
    RemoteWtn6(): ::RemoteWtn6(Packet::get()) {}
};
}

