/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-03-23     imgcr       the first version
 */

#include "local_fal_persistent_storage.hxx"

//服务侧
LocalFalPersistentStorage::LocalFalPersistentStorage(std::shared_ptr<FalPersistentStorage> storage, std::shared_ptr<Rpc> rpc): storage(storage), rpc(rpc) {

}
