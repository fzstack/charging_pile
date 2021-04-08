/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-04-01     imgcr       the first version
 */

#include "app_state.hxx"
#include <config/bsp.hxx>
#include <config/co.hxx>
#include <components/air_components.hxx>

AppState::AppState(std::shared_ptr<Packet> packet, std::shared_ptr<Air724> air) {
    packet->on<Packets::State>([this](auto p) {
        portStates[p->port.get()].state = p->value;
        portStateChanged(p->port, p->value);
    });

    air->oInited += [this, air](auto value) {
        auto ess = air->make<AirEssential>();
        iccid = ess->getIccid();
        imei = ess->getImei();
    };

    timer.onRun += [this, air](){
        if(!*cloudConnected) return;
        auto ess = air->make<AirEssential>();
        signal = ess->getCsq();
    };

    timer.start();
}

State::Value AppState::getPortState(InnerPort port) {
    return portStates[port.get()].state;
}
