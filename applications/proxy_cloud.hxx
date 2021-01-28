/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-01-27     imgcr       the first version
 */
#ifndef APPLICATIONS_PROXY_CLOUD_HXX_
#define APPLICATIONS_PROXY_CLOUD_HXX_

#include "cloud.hxx"
#include <memory>
#include <components/packet.hxx>

class ProxyCloud: public Cloud {
public:
    ProxyCloud(std::shared_ptr<Packet> packet);

    //TODO: TLV的轮子
    virtual void init() override;
    virtual void setCurrentData(std::array<CurrentData, Config::Bsp::kPortNum>& data) override;

    //下面两个函数是云类内部调用的
    virtual void setIccid(std::string_view iccid) override;
    virtual void setSignal(int signal) override;

    virtual void emitPortAccess(int port) override;
    virtual void emitPortUnplug(int port) override;
    virtual void emitIcNumber(int port, std::string_view icNumber) override;
    virtual void emitCurrentLimit(int port) override;

protected:
    virtual void setSignalInterval() override;

private:
    std::shared_ptr<Packet> packet;
};

#include <utilities/singleton.hxx>
#include <components/packet.hxx>
namespace Preset {
class ProxyCloud: public Singleton<ProxyCloud>, public ::ProxyCloud {
    friend class Singleton<ProxyCloud>;
    ProxyCloud(): ::ProxyCloud(Packet::get()) {};
};
}

#endif /* APPLICATIONS_PROXY_CLOUD_HXX_ */
