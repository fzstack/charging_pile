#include "remote_thing.hxx"
#include <config/co.hxx>
#include <stdexcept>

using namespace std;
using namespace Packets;
using namespace Rpcs;

RemoteThing::RemoteThing(shared_ptr<Packet> packet, shared_ptr<Rpc> rpc): packet(packet), rpc(rpc) {
    packet->on<Events::PortAccess>([this](auto p) {
        onPortAccess(p->port);
    });
    packet->on<Events::PortUnplug>([this](auto p) {
       onPortUnplug(p->port);
    });
    packet->on<Events::CurrentLimit>([this](auto p) {
        onCurrentLimit(p->port);
    });
    packet->on<Events::CurrentData>([this](auto p) {
        onCurrentData(p->value);
    });
}

void RemoteThing::query() {
    rpc->invoke<Services::Query>({});
}

void RemoteThing::control(InnerPort port, int timerId, int minutes) {
    rpc->invoke<Services::Control>({port, timerId, minutes});
}

void RemoteThing::stop(InnerPort port, int timerId) {
    rpc->invoke<Services::Stop>({port, timerId});
}

void RemoteThing::config(int currentLimit, int uploadThr, int fuzedThr, int noloadCurrThr) {
    rpc->invoke<Services::Config>({currentLimit, uploadThr, fuzedThr, noloadCurrThr});
}

