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
    Meta::create(size);
}

void PersistentStorage::test() {
    format();
    auto addr = alloc(4);
    rt_kprintf("addr: %02x", addr);
}

rt_uint16_t PersistentStorage::alloc(std::size_t size) {
    auto guard = getCtxGuard();
    auto head = Meta::get();
    auto allocSize = size + sizeof(HeapNode);

    //从空闲链表中找到最合适的节点
    auto curIdle = head->idle.getFront();
    auto minDiffNode = Idx<HeapNode>();
    do {
        if(minDiffNode == nullptr) {
            if(curIdle->size >= allocSize) {
                minDiffNode = curIdle;
            }
        } else {
            if(allocSize <= curIdle->size && curIdle->size < minDiffNode->size) {
                minDiffNode = curIdle;
            }
        }
        curIdle = curIdle->next;
    } while(curIdle != head->idle.getFront());

    if(minDiffNode == nullptr) {
        throw runtime_error{"alloc failed"};
    }
    auto minDiffNodeOriSize = minDiffNode->size;
    minDiffNode->size = size;

    head->idle.remove(minDiffNode);
    head->alloc.add(minDiffNode);

    auto newIdleNode = Idx<HeapNode>(minDiffNode.get() + sizeof(HeapNode) + size)(minDiffNodeOriSize - size - sizeof(HeapNode));
    head->idle.add(newIdleNode);

    return minDiffNode.get() + sizeof(HeapNode);
}

void PersistentStorage::free(rt_uint16_t addr) {

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

PersistentStorage::Meta::Meta(std::size_t deviceSize) {
    auto node = Idx<HeapNode>(sizeof(Meta))(deviceSize - sizeof(Meta) - sizeof(HeapNode));
    idle.add(node);
}

PersistentStorage::Idx<PersistentStorage::Meta> PersistentStorage::Meta::get() {
    auto meta = Idx<Meta>((rt_uint16_t)0);
    if(meta->magic != typeid(Meta).hash_code())
        throw std::runtime_error{"invalid meta"};
    return meta;
}

PersistentStorage::Idx<PersistentStorage::Meta> PersistentStorage::Meta::create(std::size_t deviceSize) {
    return Idx<Meta>((rt_uint16_t)0)(deviceSize);
}

void PersistentStorage::HeapList::add(Idx<HeapNode> node) {
    if(front == nullptr) {
        front = node;
        node->next = node;
        node->prev = node;
    } else {
        auto rear = front->prev;
        rear->next = node;
        node->prev = rear;
        front->prev = node;
        node->next = front;
    }
}

void PersistentStorage::HeapList::remove(Idx<HeapNode> node) {
    if(front == node) {
        if(front == front->next) { //仅剩一个节点
            front = nullptr;
        } else {
            front = front->next;
        }
    } else {
        auto nodePrev = node->prev;
        auto nodeNext = node->next;
        nodePrev->next = nodeNext;
        nodeNext->prev = nodePrev;
    }
}

PersistentStorage::Idx<PersistentStorage::HeapNode> PersistentStorage::HeapList::getFront() {
    return front;
}
