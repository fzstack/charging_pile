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

#include "thing.hxx"
#include <memory>
#include <components/packet.hxx>
#include <components/rpc.hxx>

class LocalThing: public Thing {
public:
    LocalThing(std::array<std::shared_ptr<Charger>, Config::Bsp::kPortNum> chargers, std::shared_ptr<User> user, std::shared_ptr<LastCharger> last, std::shared_ptr<Packet> packet, std::shared_ptr<Rpc> rpc);
private:
    std::shared_ptr<Packet> packet;
    std::shared_ptr<Rpc> rpc;
};

#include <utilities/singleton.hxx>
namespace Preset {
class LocalThing: public Singleton<LocalThing>, public ::LocalThing {
    friend class Singleton<LocalThing>;
    LocalThing();
};
}
