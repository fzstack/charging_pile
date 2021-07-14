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

#include <components/packet.hxx>
#include <memory>
#include <components/timer.hxx>
#include <utilities/signals.hxx>
#include <utilities/observable.hxx>
#include <utilities/observer.hxx>

class Packet;

class Handshake {
public:
    Handshake(std::shared_ptr<Packet> packet);
    void hello();

private:
    Observable<bool> connected;
public:
    Observer<bool> oConnected = {connected};

private:
    std::shared_ptr<Packet> packet;
    bool responsed = false;
    bool once = false;
    Timer timer = {1000, "hsk"};
};

#include <utilities/singleton.hxx>
namespace Preset {
class Handshake: public Singleton<Handshake>, public ::Handshake {
    friend singleton_t;
    Handshake();
};
}

