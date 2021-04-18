#pragma once

#include "thing.hxx"
#include "thing_base.hxx"
#include <memory>
#include <components/packet.hxx>
#include <components/rpc.hxx>
#include <utilities/shared_thread.hxx>
#include <utilities/shared_thread.hxx>

class LocalThing: public ThingBase {
public:
    LocalThing(std::shared_ptr<Thing> thing, std::shared_ptr<Packet> packet, std::shared_ptr<Rpc> rpc, std::shared_ptr<SharedThread> thread);
    virtual void init() override;
    virtual void query() override;
    virtual void control(InnerPort port, int timerId, int minutes) override;
    virtual void stop(InnerPort port, int timerId) override;
    virtual void config(DevConfig conf) override;
    virtual DevConfig readConfig() override;

private:
    std::shared_ptr<Thing> thing;
    std::shared_ptr<Packet> packet;
    std::shared_ptr<Rpc> rpc;
    std::shared_ptr<SharedThread> thread;
};

#ifdef LOWER_END
#include <utilities/singleton.hxx>
namespace Preset {
class LocalThing: public Singleton<LocalThing>, public ::LocalThing {
    friend class Singleton<LocalThing>;
    LocalThing(): ::LocalThing(ThingPre::get(), Packet::get(), Rpc::get(), SharedThread<Priority::Middle>::get()) { }
};
}
#endif
