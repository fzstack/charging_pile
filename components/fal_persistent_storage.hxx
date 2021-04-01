#pragma once
#include <memory>
#include <string>

#include <rtthread.h>
#include <flashdb.h>

#include <utilities/memory_iostream.hxx>
#include <utilities/serializer.hxx>
#include <utilities/deserializer.hxx>
#include <utilities/f.hxx>

#include <utilities/tiny_type_id.hxx>

#include <Mutex.h>
#include <Lock.h>

class FalPersistentStorage {
public:
    FalPersistentStorage(const char *spiBus, const char *spiDev, rt_base_t ssPin, fdb_kvdb_t db, 
        const char *dbName, const char *flashName, const char *partName);

    template<class T>
    std::shared_ptr<T> make() {
        return std::shared_ptr<T>(new T(read<T>()), [=](auto p){
            write<T>(std::move(*p));
#ifdef TEST_PERSISTENT_STORAGE
            F{} << "config done"_r << endln;
#endif
            delete p;
        });
    }

    template<class T>
    T read() {
        struct fdb_kv kv;
        auto encoded = toHex(TypeId<T>::get());
        auto guard = rtthread::Lock(mutex);
        auto result = fdb_kv_get_obj(db, encoded.c_str(), &kv);
        if(result == nullptr) { //配置不存在
            //直接返回初始化的T
            return {};
        } else {
            //读取配置并返回
            auto value = std::vector<rt_uint8_t>(kv.len);
            fdb_blob blob;
            fdb_kv_get_blob(db, encoded.c_str(), fdb_blob_make(&blob, &value[0], kv.len));

            //反序列化
            auto iostream = std::make_shared<MemoryIOStream>();
            iostream->writeData(&value[0], kv.len);
            auto des = Deserializer{iostream};
            return des.parse<T>();
        }
    }

    template<class T>
    void write(T&& t) {
        //序列化后保存
        fdb_blob blob;
        auto encoded = toHex(TypeId<T>::get());
        auto ostream = std::make_shared<MemoryIOStream>();
        auto ser = Serializer{ostream};
        ser.build(t);
        auto buf = ostream->getBuffer();
        auto guard = rtthread::Lock(mutex);
        fdb_kv_set_blob(db, encoded.c_str(), fdb_blob_make(&blob, &buf[0], buf.size()));
    }

    void reset() {
        fdb_kv_set_default(db);
    }

private:
    std::string toBase64(size_t hash);
    std::string toHex(size_t hash);
    fdb_kvdb_t db;
    rtthread::Mutex mutex;
};

#include <utilities/singleton.hxx>
namespace Preset {
class FalPersistentStorage: public Singleton<FalPersistentStorage>, public ::FalPersistentStorage {
    friend singleton_t;
    FalPersistentStorage(): ::FalPersistentStorage(kSpiBus, kSpiDev, kSsPin, 
      kDb, kDbName, kFlashName, kPartName) {

    }
    const static char *kSpiBus, *kSpiDev;
    const static rt_base_t kSsPin;
    const static fdb_kvdb_t kDb;
    const static char *kFlashName, *kDbName, *kPartName;
};
}

