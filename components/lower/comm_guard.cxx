#include "comm_guard.hxx"

CommGuard::CommGuard(std::shared_ptr<Handshake> handshake, std::shared_ptr<WatchDog> watchDog): handshake(handshake), watchDog(watchDog) {
    handshake->onHandshake += [=](auto type) {
        if(type != handshake->type)
            return;
        count.reset();
        count.retrigger();
    };

    timer.onRun += [=] {
        if(count.updateAndCheck()) {
            rt_kprintf("no more handshake\n");
            watchDog->resetAfter(1);
        }
    };
    timer.start();
}
