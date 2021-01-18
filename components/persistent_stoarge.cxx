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

PersistentStorage::MakeResult PersistentStorage::makeInternal(std::size_t hash, std::size_t size) {
    //遍历type链表，找到hash code相同的节点
    auto head = Meta::get();
    if(!head->isValid()) {
        format();
    }

    auto& type = head->type;
    auto found = std::find_if(type.begin(), type.end(), [&](Idx<TypeNode> node){
        return node->hash == hash;
    });

    auto typeNode = Idx<TypeNode>{};
    if(found == type.end()) { //没有找到、添加节点
        typeNode = Idx<TypeNode>{alloc(sizeof(TypeNode))}(hash);
        type.add(typeNode);
    } else {
        typeNode = *found;
    }

    //判断type中的obj链表是否至少包含一个节点
    auto& obj = typeNode->obj;
    auto objNode = obj.getFront();
    bool created = true;
    if(objNode == nullptr) { //不存在、添加新节点
        objNode = Idx<ObjNode>{alloc(sizeof(ObjNode) + size)}();
        obj.add(objNode);
        created = false;
    }
    return MakeResult {created, rt_uint16_t(objNode.get() + sizeof(ObjNode))};
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
//    if(newIdleNode.get() == 0x3e)
//        throw not_implemented{};
    head->idle.add(newIdleNode);

    return minDiffNode.get() + sizeof(HeapNode);
}

void PersistentStorage::free(rt_uint16_t addr) {
    throw not_implemented{""};
}

std::shared_ptr<void> PersistentStorage::getCtxGuard() {

    if(!owner) {
        owner = make_shared<IdxOwner>(shared_from_this());
    }

    if(holder.expired()) {
        IdxOwner::owner = owner;
        auto guard = std::shared_ptr<void>(nullptr, [](auto) {
            IdxOwner::owner = std::shared_ptr<IdxOwner>(nullptr);
        });
        holder = guard;
        return guard;
    }

    return holder.lock();
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

