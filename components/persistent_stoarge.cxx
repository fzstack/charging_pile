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
    Meta::make(size);
}

void PersistentStorage::test() {
    rt_kprintf("=======format=======\n");
    format();
    rt_kprintf("=======alloc=======\n");
    auto addr1 = alloc(4);
    rt_kprintf("addr1: %02x\n", addr1);
    auto addr2 = alloc(4);
    rt_kprintf("addr2: %02x\n", addr2);
    auto addr3 = alloc(4);
    rt_kprintf("addr2: %02x\n", addr3);
}

rt_uint16_t PersistentStorage::alloc(std::size_t size) {
    auto guard = getCtxGuard();
    auto head = Meta::get();
    auto allocSize = size + sizeof(HeapNode);

    //从空闲链表中找到最合适的节点
    rt_kprintf("get front\n");
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

void PersistentStorage::Meta::create(Meta& self, std::size_t deviceSize) {
    self = Meta{};
    auto node = Idx<HeapNode>(sizeof(Meta))(deviceSize - sizeof(Meta) - sizeof(HeapNode));
    self.idle.add(node);
    ///此处node被析构
    ///我们希望Node内的Idx也被析构
    ///
}

PersistentStorage::Idx<PersistentStorage::Meta> PersistentStorage::Meta::get() {
    auto meta = Idx<Meta>((rt_uint16_t)0);
    if(meta->magic != typeid(Meta).hash_code())
        throw std::runtime_error{"invalid meta"};
    return meta;
}

PersistentStorage::Idx<PersistentStorage::Meta> PersistentStorage::Meta::make(std::size_t deviceSize) {
    return Idx<Meta>((rt_uint16_t)0)(deviceSize);
}

void PersistentStorage::HeapList::add(Idx<HeapNode> node) {
    if(front == nullptr) {
        rt_kprintf("add head\n");
        front = node;
        //TODO: 下面两次赋值会导致bug发生
        ///因为node->next持有了node自身的Idx, 导致引用计数 + 2
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
    }
    auto nodePrev = node->prev;
    auto nodeNext = node->next;
    nodePrev->next = nodeNext;
    nodeNext->prev = nodePrev;
}

PersistentStorage::Idx<PersistentStorage::HeapNode> PersistentStorage::HeapList::getFront() {
    return front;
}
