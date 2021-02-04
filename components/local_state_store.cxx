/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-02-03     imgcr       the first version
 */

#include "local_state_store.hxx"
#include <components/packet.hxx>
#include <config/bsp.hxx>
#include <config/co.hxx>
#include <utilities/mp.hxx>

LocalStateStore::LocalStateStore(int idx) {
    oState += [idx](auto value) {
        if(!value) return;
        auto packet = Preset::Packet::get();
        magic_switch<Config::Bsp::kPortNum>{}([&](auto v) {
            packet->emit<Packets::State<decltype(v)::value>>({*value});
        }, idx);
    };
}