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

using namespace std;

ThreadLocal<std::shared_ptr<typename PersistentStorage::IdxOwner>> PersistentStorage::IdxOwner::owner;

PersistentStorage::PersistentStorage(at24cxx_device_t device, int size): device(device), size(size) {

}

void PersistentStorage::format() {
    auto guard = getCtxGuard();

    auto head = MetaHead::get();
    auto idle = Idx<IdleNode>(sizeof(MetaHead));

    idle->size = size - sizeof(MetaHead);  //整个Idle节点的大小，包括这个结构体本身
    idle->next = nullptr;
    idle->prev = nullptr;

    head->magic = typeid(MetaHead).hash_code();
    head->idle = idle;
    head->alloc = nullptr;
}

void PersistentStorage::test() {
    auto guard = getCtxGuard();

    auto head = MetaHead::get();
    rt_kprintf("idle size: %d\n", head->idle->next->size);
}

void PersistentStorage::alloc() {
    auto guard = getCtxGuard();


}

std::shared_ptr<void> PersistentStorage::getCtxGuard() {

    if(!owner) {
        owner = make_shared<IdxOwner>(shared_from_this());
    }

    IdxOwner::owner = owner;
    return std::shared_ptr<void>(nullptr, [](auto) {
        IdxOwner::owner = std::shared_ptr<IdxOwner>(nullptr);
    });
}
