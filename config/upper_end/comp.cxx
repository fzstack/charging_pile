/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-03-20     imgcr       the first version
 */

#ifdef UPPER_END

#include <components/fal_persistent_storage.hxx>

extern "C" {
fdb_kvdb _global_kvdb;
}

namespace Preset {

const fdb_kvdb_t FalPersistentStorage::kDb = &_global_kvdb;
const char
    *FalPersistentStorage::kFlashName = FAL_USING_NOR_FLASH_DEV_NAME,
    *FalPersistentStorage::kDbName = "env",
    *FalPersistentStorage::kPartName = "fdb_kvdb";


}


#endif

