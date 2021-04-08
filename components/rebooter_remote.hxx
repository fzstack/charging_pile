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

class RebooterRemote: public Rebooter {
public:
    RebooterRemote(std::shared_ptr<Packet> packet);
    virtual void reboot() override;
};

#include <utilities/singleton.hxx>
namespace Preset {
class RebooterRemote: public Singleton<RebooterRemote>, public ::RebooterRemote {
    friend singleton_t;
    RebooterRemote(): ::RebooterRemote(Packet::get()) { }
};
}
