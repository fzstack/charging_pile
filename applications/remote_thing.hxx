#pragma once

#include "thing_base.hxx"
#include <components/packet.hxx>
#include <components/rpc.hxx>
#include <memory>

class RemoteThing: public ThingBase {
public:
    RemoteThing(std::shared_ptr<Packet> packet, std::shared_ptr<Rpc> rpc);

    virtual void init() override;
    virtual void query() override;
    virtual void control(InnerPort port, int timerId, int minutes) override;
    virtual void stop(InnerPort port, int timerId) override;
    virtual void config(int currentLimit, int uploadThr, int fuzedThr, int noloadCurrThr) override;
private:
    std::shared_ptr<Packet> packet;
    std::shared_ptr<Rpc> rpc;
};

#include <utilities/singleton.hxx>
namespace Preset {
class RemoteThing: public Singleton<RemoteThing>, public ::RemoteThing {
    friend class Singleton<RemoteThing>;
    RemoteThing(): ::RemoteThing(Packet::get(), Rpc::get()) {}
};
}
