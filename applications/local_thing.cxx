/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-02-05     imgcr       the first version
 */

#include "local_thing.hxx"
#include <applications/cloud.hxx>
#include <config/co.hxx>
#include <stdexcept>

using namespace std;
using namespace Rpcs;
using namespace Packets;

LocalThing::LocalThing(
    std::array<std::shared_ptr<Charger>, Config::Bsp::kPortNum> chargers,
    std::shared_ptr<User> user,
    std::shared_ptr<LastCharger> last,
    std::shared_ptr<Packet> packet,
    std::shared_ptr<Rpc> rpc
): Thing(chargers, user, last), packet(packet), rpc(rpc) {
    rpc->def<Services::Control>([this](auto p, auto r) {
        try {
            control(p->port, p->timerId, p->minutes);
            r(true);
        } catch(const exception& e) {
            r(false);
        }
    });
    rpc->def<Services::Stop>([this](auto p, auto r) {
       try {
           stop(p->port, p->timerId);
           r(true);
       } catch(const exception& e) {
           r(false);
       }
    });
    rpc->def<Services::Config>([this](auto p, auto r) {
        config(p->currentLimit, p->uploadThr, p->fuzedThr, p->noloadCurrThr);
        r({});
    });

    onPortAccess += [this](auto port) {
        this->packet->emit<Events::PortAccess>({port});
    };
    onPortUnplug += [this](auto port) {
        this->packet->emit<Events::PortUnplug>({port});
    };
    onIcNumber += [this](auto port, auto icNumber) {
        auto p = Events::IcNumber{};
        p.port = port;
        strncpy(p.icNumber, icNumber.c_str(), sizeof(p.icNumber));
        this->packet->emit(p);
    };
    onCurrentLimit += [this](auto port) {
        this->packet->emit<Events::CurrentLimit>({port});
    };

    //TODO: get Current Data
}

#ifdef LOWER_END
#include <things/decos/counter.hxx>
#include <things/decos/event_emitter.hxx>
#include <things/decos/current_limiter.hxx>
#include <things/decos/backuper.hxx>
#include <things/decos/data_setter.hxx>
#include <things/decos/consumption_measurer.hxx>
#include <things/decos/fuse_detecter.hxx>
#include <things/decos/noload_detecter.hxx>
using namespace Things::Decos;
namespace Preset {
LocalThing::LocalThing(): ::LocalThing(Chargers::get(), /*User::get()*/nullptr, LastCharger::get(), Packet::get(), Rpc::get()) {
    addDeco<EventEmitter>();
    addDeco<Counter>();
    //addDeco<CurrentLimiter>();
    //addDeco<Backuper>();
    //addDeco<DataSetter>();
    addDeco<ConsumptionMeasurer>();
    //addDeco<FuseDetecter>();
    //addDeco<NoloadDetecter>();
    init();
}
}
#endif
