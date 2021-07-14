#include "lower_conf.hxx"
#include <flashdb.h>
#include <config/app.hxx>

extern "C" {
#include <crc.h>
}


static DevConfig defualtConf = {
    currentLimit: 4000,
    uploadThr: 100,
    fuzedThr: 0,
    noloadCurrThr: 44,
    doneCurrThr: 80,
};

// static fdb_default_kv_node defaultConfTab[] = {
//     {
//         key: (char*)LowerConf::confName,
//         value: &defualtConf,
//         value_len: sizeof(defualtConf),
//     }
// };

// static fdb_default_kv defaultConfKv = {
//     kvs: defaultConfTab,
//     num: 1,
// };

LowerConf::LowerConf(std::shared_ptr<Fal> fal): part(fal_partition_find(partName)) {
    
}

DevConfig LowerConf::read() {
    auto meta = Meta<DevConfig>{};
    fal_partition_read(part, 0, (uint8_t*)&meta, sizeof(meta));
    auto crcVal = crc16_cal((uint8_t*)&meta.data, sizeof(meta.data));
    if(crcVal != meta.hash) {
        return defualtConf;
    }
    return meta.data;
    
}

void LowerConf::write(DevConfig& conf) {
    auto meta = Meta<DevConfig>{};
    meta.data = conf;
    meta.hash = crc16_cal((uint8_t*)&meta.data, sizeof(meta.data));
    fal_partition_erase(part, 0, sizeof(meta));
    fal_partition_write(part, 0, (uint8_t*)&meta, sizeof(meta));
}

const char* LowerConf::partName = "conf";