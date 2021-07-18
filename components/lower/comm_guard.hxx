#pragma once
#include <components/handshake.hxx>
#include <components/watch_dog.hxx>
#include <components/timer.hxx>
#include <utilities/count_down.hxx>

class CommGuard {
public:
    CommGuard(std::shared_ptr<Handshake> handshake, std::shared_ptr<WatchDog> watchDog);

private:
    std::shared_ptr<Handshake> handshake;
    std::shared_ptr<WatchDog> watchDog;
    Timer timer = {2000, "cg"};
    CountDown<> count = {3};
};

#include <utilities/singleton.hxx>
namespace Preset {
class CommGuard: public Singleton<CommGuard>, public ::CommGuard {
    friend singleton_t;
    CommGuard(): ::CommGuard(Handshake::get(), WatchDog::get()) {}
};
}