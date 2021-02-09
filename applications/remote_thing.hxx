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

#include "thing_base.hxx"
#include <components/packet.hxx>
#include <components/rpc.hxx>
#include <memory>

class RemoteThing: public ThingBase {
public:
    RemoteThing(std::shared_ptr<Packet> packet, std::shared_ptr<Rpc> rpc);
    virtual void control(int port, int timerId, int minutes) override;
    virtual void stop(int port, int timerId) override;
    virtual void config(int currentLimit, int uploadThr, int fuzedThr, int noloadCurrThr) override;
    virtual std::array<CurrentData, Config::Bsp::kPortNum> getCurrentData() override;
private:
    std::shared_ptr<Packet> packet;
    std::shared_ptr<Rpc> rpc;
};

#include <utilities/singleton.hxx>
namespace Preset {
class RemoteThing: public Singleton<RemoteThing>, public ::RemoteThing {
    friend class Singleton<RemoteThing>;
    RemoteThing(): ::RemoteThing(Packet::get(), Rpc::get()) {}
};
}
