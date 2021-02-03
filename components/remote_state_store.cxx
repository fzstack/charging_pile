/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-02-03     imgcr       the first version
 */

#include "remote_state_store.hxx"
#include <components/packet.hxx>
#include <config/bsp.hxx>
#include <config/co.hxx>
#include <utilities/mp.hxx>

RemoteStateStore::RemoteStateStore(int idx) {
    auto packet = Preset::Packet::get();
    magic_switch<Config::Bsp::kPortNum>{}([&](auto v) {
        packet->on<Packets::State<decltype(v)::value>>([this](auto p){
            this->state = p->value;
        });
    }, idx);
}

