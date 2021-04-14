/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-04-08     imgcr       the first version
 */
#pragma once

#include "rebooter.hxx"
#include <components/packet.hxx>
#include <devices/air724.hxx>

class RebooterLocal: public Rebooter {
public:
    RebooterLocal(std::shared_ptr<Air724> air724, std::shared_ptr<Packet> packet);
    virtual void reboot() override;
    virtual void rebootAll() override;

    std::shared_ptr<Air724> air724;
    std::shared_ptr<Packet> packet;
};

#include <utilities/singleton.hxx>
namespace Preset {
class RebooterLocal: public Singleton<RebooterLocal>, public ::RebooterLocal {
    friend singleton_t;
    RebooterLocal(): ::RebooterLocal(Air724::get(), Packet::get()) { }
};
}
