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
#include "rc522_base.hxx"
#include <components/packet.hxx>

class RemoteRc522: public Rc522Base {
public:
    RemoteRc522(std::shared_ptr<Packet> packet);
private:
    std::shared_ptr<Packet> packet;
};

#include <utilities/singleton.hxx>
namespace Preset {
class RemoteRc522: public Singleton<RemoteRc522>, public ::RemoteRc522 {
    friend singleton_t;
    RemoteRc522(): ::RemoteRc522(Packet::get()) {}
};
}
