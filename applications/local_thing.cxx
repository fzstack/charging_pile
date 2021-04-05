#include "local_thing.hxx"
#include <config/co.hxx>
#include <stdexcept>

using namespace std;
using namespace Rpcs;
using namespace Packets;

#define LOG_LT_INVOKE

LocalThing::LocalThing(
    std::shared_ptr<Thing> thing,
    std::shared_ptr<Packet> packet,
    std::shared_ptr<Rpc> rpc
): thing(thing), packet(packet), rpc(rpc) {
    packet->on<Services::Control>([this](auto p) {
        control(p->port, p->timerId, p->minutes);
    });
    packet->on<Services::Stop>([this](auto p) {
        stop(p->port, p->timerId);
    });
    packet->on<Services::Config>([this](auto p){
        config(p->conf);
    });
    packet->on<Services::Query>([this](auto p) {
#ifdef LOG_LT_INVOKE
        rt_kprintf("on remote query\n");
#endif
        query();
    });

    thing->onPortAccess += [this](auto port) {
        onPortAccess(port);
        this->packet->emit<Events::PortAccess>({port});
    };

    thing->onPortUnplug += [this](auto port) {
        onPortUnplug(port);
        this->packet->emit<Events::PortUnplug>({port});
    };

    thing->onCurrentLimit += [this](auto port) {
        onCurrentLimit(port);
        this->packet->emit<Events::CurrentLimit>({port});
    };

    thing->onCurrentData += [this](auto data){
        onCurrentData(data);
        this->packet->emit<Events::CurrentData>({data});
    };
}

void LocalThing::init() {
    thing->init();
}

void LocalThing::query() {
    thing->query();
}

void LocalThing::control(InnerPort port, int timerId, int minutes) {
    thing->control(port, timerId, minutes);
}

void LocalThing::stop(InnerPort port, int timerId) {
    thing->stop(port, timerId);
}

void LocalThing::config(DevConfig conf) {
    thing->config(conf);
}

DevConfig LocalThing::readConfig() {
    return thing->readConfig();
}


