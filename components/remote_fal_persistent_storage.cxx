#include "remote_fal_persistent_storage.hxx"

RemoteFalPersistentStorage::RemoteFalPersistentStorage(std::shared_ptr<Rpc> rpc, std::shared_ptr<Packet> packet): rpc(rpc), packet(packet) {

}

