#include "rpc.hxx"

using namespace std;
using namespace rtthread;

Rpc::Rpc(shared_ptr<Packet> packet, std::shared_ptr<SharedThread> thread): packet(packet), thread(thread) {
    timer.onRun += [this]{
        auto guard = Lock{mutex};
        auto currTick = rt_tick_get();
        for(auto it = pendings.begin(); it != pendings.end();) {
            auto pending = it->second;
            if(currTick - pending->createdTick > pending->timeout) {
                pending->emitTimeout();
                it = pendings.erase(it);
            } else {
                ++it;
            }
        }
    };

    timer.start();
}

