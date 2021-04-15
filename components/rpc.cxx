#include "rpc.hxx"

using namespace std;
using namespace rtthread;

Rpc::Rpc(shared_ptr<Packet> packet): packet(packet) {
    timer.onRun += [this]{
        auto guard = Lock{mutex};
        auto currTick = rt_tick_get();
        for(auto it = pendings.begin(); it != pendings.end();) {
            auto pending = it->second;
            if(currTick - pending->createdTick > kTimeout) {
                pending->timeout();
                it = pendings.erase(it);
            } else {
                ++it;
            }
        }
    };

    timer.start();
}

