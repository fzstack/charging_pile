/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-01-04     imgcr       the first version
 */

#include "persistent_storage.hxx"

ThreadLocal<std::shared_ptr<PersistentStorage>> PersistentStorage::self = std::shared_ptr<PersistentStorage>(nullptr);

PersistentStorage::PersistentStorage(at24cxx_device_t device, int size): device(device), size(size) {

}

void PersistentStorage::format() {
    auto guard = getCtxGuard();

    auto head = MetaHead::get();
    auto idle = Idx<IdleNode>(sizeof(MetaHead));

    rt_kprintf("idx size: %d\n", sizeof(Idx<IdleNode>));
    idle->size = size - sizeof(MetaHead);
    idle->next = nullptr;
    idle->prev = nullptr;

    head->magic = typeid(MetaHead).hash_code();
    head->idle = idle;
    head->alloc = nullptr;
}

void PersistentStorage::alloc() {
    auto guard = getCtxGuard();


}

std::shared_ptr<void> PersistentStorage::getCtxGuard() {
    self = shared_from_this();
    return std::shared_ptr<void>(nullptr, [](auto) {
        self = std::shared_ptr<PersistentStorage>(nullptr);
    });
}
