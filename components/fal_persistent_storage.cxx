#include "fal_persistent_storage.hxx"
#include <devices/pin_map.hxx>

#include <board.h>
extern "C" {
#include <drv_spi.h>
#include <spi_flash_sfud.h>
#include <fal.h>
}

#include <tinycrypt.h>

#include <Mutex.h>
#include <Lock.h>

using namespace std;

rtthread::Mutex mutex;

static void lock(fdb_db_t db) {
    mutex.lock();
}

static void unlock(fdb_db_t db) {
    mutex.unlock();
}

FalPersistentStorage::FalPersistentStorage(const char *spiBus, const char *spiDev, rt_base_t ssPin, fdb_kvdb_t db, const char *dbName, const char *flashName, const char *partName): db(db) {
    rt_hw_spi_device_attach(spiBus, spiDev, pins[ssPin].gpio, pins[ssPin].pin);
    rt_sfud_flash_probe(flashName, spiDev);

    fdb_kvdb_control(db, FDB_KVDB_CTRL_SET_LOCK, (void*)lock);
    fdb_kvdb_control(db, FDB_KVDB_CTRL_SET_UNLOCK, (void*)unlock);

    auto def_kv = fdb_default_kv{num: 0};
    fdb_kvdb_init(db, dbName, partName, &def_kv, NULL);
}

std::shared_ptr<void> FalPersistentStorage::makeInternal(size_t id) {
    return make_shared<int>();
}

string FalPersistentStorage::toBase64(size_t hash) {
    auto result = string(10, '\0');
    int dlen;
    tiny_base64_encode((unsigned char*)(void*)result.c_str(), &dlen, (unsigned char*)(void*)&hash, sizeof(hash));
    return result;
}

string FalPersistentStorage::toHex(size_t hash) {
    auto result = string(10, '\0');
    rt_sprintf((char*)result.c_str(), "%08x", hash);
    return result;
}


