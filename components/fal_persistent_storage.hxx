/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-03-20     imgcr       the first version
 */
#pragma once
#include <memory>
#include <string>

#include <rtthread.h>
#include <flashdb.h>

#include <utilities/memory_iostream.hxx>
#include <utilities/serializer.hxx>
#include <utilities/deserializer.hxx>

class FalPersistentStorage {
public:
    FalPersistentStorage(const char *spiBus, const char *spiDev, rt_base_t ssPin, fdb_kvdb_t db, 
        const char *dbName, const char *flashName, const char *partName);

    template<class T>
    std::shared_ptr<T> make() {
        //1.hash_code 转 base64
        auto encoded = toBase64(typeid(T).hash_code());
        rt_kprintf("code: %08x, encoded: %s\n", typeid(T).hash_code(), encoded.c_str());

        //2.判断配置是否存在，如果不存在，则使用默认配置
        struct fdb_kv kv;
        auto result = fdb_kv_get_obj(db, encoded.c_str(), &kv);

        auto saver = [=](auto p){
            //序列化后保存
            fdb_blob blob;
            auto ostream = std::make_shared<MemoryIOStream>();
            auto ser = Serializer{ostream};
            ser.build(*(T*)p);
            auto buf = ostream->getBuffer();
            fdb_kv_set_blob(db, encoded.c_str(), fdb_blob_make(&blob, &buf[0], buf.size()));
        };

        if(result == nullptr) { //配置不存在
            //直接返回初始化的T
            return std::shared_ptr<T>(new T(), saver);
        } else {
            //读取配置并返回
            auto value = std::vector<rt_uint8_t>(kv.len);
            fdb_blob blob;
            fdb_kv_get_blob(db, encoded.c_str(), fdb_blob_make(&blob, &value[0], kv.len));

            //反序列化
            auto iostream = std::make_shared<MemoryIOStream>();
            iostream->writeData(&value[0], kv.len);
            auto des = Deserializer{iostream};
            return std::shared_ptr<T>(new T(des.parse<T>()), saver);
        }
    }

    std::shared_ptr<void> makeInternal(size_t id);

private:
    std::string toBase64(size_t hash);
    fdb_kvdb_t db;
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

