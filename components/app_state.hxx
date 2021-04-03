/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-04-01     imgcr       the first version
 */
#pragma once
#include <utilities/signals.hxx>
#include <utilities/inner_port.hxx>
#include <config/app.hxx>
#include <config/bsp.hxx>
#include <array>
#include <components/packet.hxx>

//TODO: do not use remote store
class AppState {
public:
    AppState(std::shared_ptr<Packet> packet);
    Signals<void(InnerPort, State::Value)> portStateChanged;
    State::Value getPortState(InnerPort port);
private:
    struct Wrapper {
        State::Value state = State::LoadNotInsert;
    };
    std::array<Wrapper, Config::Bsp::kPortNum> portStates;
};

#include <utilities/singleton.hxx>
namespace Preset {
class AppState: public Singleton<AppState>, public ::AppState {
    friend singleton_t;
    AppState(): ::AppState(Packet::get()) { }
};
}
