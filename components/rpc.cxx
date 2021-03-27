#include "rpc.hxx"

Rpc::Rpc(std::shared_ptr<Packet> packet, std::shared_ptr<SharedThread> thread): packet(packet), thread(thread) {

}

