#include "local_fal_persistent_storage.hxx"

//服务侧
LocalFalPersistentStorage::LocalFalPersistentStorage(std::shared_ptr<FalPersistentStorage> storage, std::shared_ptr<Rpc> rpc, std::shared_ptr<Packet> packet): storage(storage), rpc(rpc), packet(packet) {

}
