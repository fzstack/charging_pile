/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-01-27     imgcr       the first version
 */

#include "proxy_cloud.hxx"
#include <config/co.hxx>

using namespace Packets;

ProxyCloud::ProxyCloud(std::shared_ptr<Packet> packet): Cloud(nullptr), packet(packet) {
    packet->on<Services::Query>([this](auto p){
        onQuery();
    });

    packet->on<Services::Control>([this](auto p){
        onControl(p->port, p->timerId, p->minutes);
    });

    packet->on<Services::Stop>([this](auto p) {
        onStop(p->port, p->timerId);
    });

    packet->on<Services::Config>([this](auto p) {
        onConfig(p->currentLimit, p->uploadThr, p->fuzedThr, p->noloadCurrThr);
    });
}

void ProxyCloud::init() {
    packet->emit<Init>();
}

void ProxyCloud::setCurrentData(std::array<CurrentData, Config::Bsp::kPortNum>& data) {
    auto prop = Props::CurrentData{};
    for(auto i = 0u; i < Config::Bsp::kPortNum; i++) {
        prop.value[i] = data[i];
    }
    packet->emit(prop);
}

void ProxyCloud::setIccid(std::string_view iccid) {
    auto prop = Props::Iccid{};
    strncpy(prop.value, iccid.data(), sizeof(prop.value));
    packet->emit(prop);
}

void ProxyCloud::setSignal(int signal) {
    packet->emit<Props::Signal>({signal});
}

void ProxyCloud::emitPortAccess(int port) {
    packet->emit<Events::PortAccess>({port});
}

void ProxyCloud::emitPortUnplug(int port) {
    packet->emit<Events::PortUnplug>({port});
}

void ProxyCloud::emitIcNumber(int port, std::string_view icNumber) {
    auto event = Events::IcNumber{port: port};
    strncpy(event.icNumber, icNumber.data(), sizeof(event.icNumber));
    packet->emit(event);
}

void ProxyCloud::emitCurrentLimit(int port) {
    packet->emit<Events::CurrentLimit>({port});
}

void ProxyCloud::setSignalInterval() {

}
