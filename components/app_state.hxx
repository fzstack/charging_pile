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
#include <components/rpc.hxx>
#include <utilities/observable.hxx>
#include <components/timer.hxx>
#include <devices/air724.hxx>
#include <string>
#include <devices/dht11.hxx>
#include <optional>

//TODO: do not use remote store
class AppState {
public:
    AppState(std::shared_ptr<Packet> packet, std::shared_ptr<Air724> air, std::shared_ptr<Dht11> dht11, std::shared_ptr<Rpc> rpc);
    Signals<void(InnerPort, State::Value)> portStateChanged;
    State::Value getPortState(InnerPort port);

    Observable<bool> cloudConnected = {false};
    Observable<int> signal = {17};
    Observable<std::optional<int>> progress;
    std::string iccid = {}, imei = {};
    std::shared_ptr<Dht11> dht11;
    std::shared_ptr<Rpc> rpc;

private:
    struct Wrapper {
        State::Value state = State::LoadNotInsert;
    };
    std::array<Wrapper, Config::Bsp::kPortNum> portStates;
    Timer timer = {5 * 1000, "appst"};
};

#include <utilities/singleton.hxx>
namespace Preset {
class AppState: public Singleton<AppState>, public ::AppState {
    friend singleton_t;
    AppState(): ::AppState(Packet::get(), Air724::get(), Dht11::get(), Rpc::get()) { }
};
}
