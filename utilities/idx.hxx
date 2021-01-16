/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-01-12     imgcr       the first version
 */
#ifndef UTILITIES_IDX_HXX_
#define UTILITIES_IDX_HXX_

#include <map>
#include <list>
#include <memory>
#include <functional>
#include "err.hxx"
#include <rtthread.h>
#include "thread_local.hxx"
#include <algorithm>
#include <boost/pfr.hpp>

//NOTE: 目前阻止部分交叠申请, 因为那需要保证buffer连续
//TODO: shared_ptr持有指针引用导致内存泄漏问题
template <class Owner, class Addr, Addr Null, Addr Max>
class IdxAsset {
public:
    IdxAsset(Addr addr = Null): addr(addr) {};
    IdxAsset(std::nullptr_t n): addr(Null) {};

    void* getPtr(std::size_t size);
    std::size_t& getRefCount(std::size_t size);
    bool isAccessed() const;
    std::shared_ptr<rt_uint8_t[]> dispose(std::size_t size);

    template<class U>
    static void printContainer(U& t) {
        rt_kprintf("{");
        for(const auto& val: t) {
            rt_kprintf("%02x, ", val);
        }
        rt_kprintf("}");
    }

    struct Field {
        std::shared_ptr<Owner> owner = Owner::get();
        std::shared_ptr<rt_uint8_t[]> current = nullptr;
    };

    struct OwnerSpec {
        std::list<Addr> range = {0, Max + 1};
        std::list<std::size_t> count = {0};

        struct Data {
            std::shared_ptr<rt_uint8_t[]> current = nullptr;
            std::shared_ptr<rt_uint8_t[]> backup = nullptr;
        };

        std::map<Addr, Data> data = {};
    };

    static std::map<void*, Field> field;
    static std::map<std::shared_ptr<Owner>, OwnerSpec> ownerSpecs;

protected:
    Addr addr;
};

template <class T, class Owner, class Addr, Addr Null, Addr Max>
class Idx: private IdxAsset<Owner, Addr, Null, Max> { //NOTE: 改变指针要调用dispose
    using asset_t = IdxAsset<Owner, Addr, Null, Max>;
public:
    using asset_t::asset_t;
    Idx(const Idx<T, Owner, Addr, Null, Max>& other) {
        asset_t::addr = other.addr;
        if(other.isAccessed()) {
            getPtr();
        }
    }

    ~Idx() {
        dispose();
        asset_t::field.erase(this);
    }

    T* operator->() { return (T*)getPtr(); }
    T& operator*() { return *(T*)getPtr(); }

    template<class... Args>
    auto operator()(Args&&... args) {
        //new(getPtr())T(std::forward<Args>(args)...);
        T::create(*(T*)getPtr(), std::forward<Args>(args)...);
        return *this;
    }

    Addr get() {
        return asset_t::addr;
    }

    void operator=(Addr addr) { dispose(); this->addr = addr; }
    void operator=(const Idx<T, Owner, Addr, Null, Max>& other) {
        dispose();
        asset_t::addr = other.addr;
        if(other.isAccessed()) {
            rt_kprintf("%08x@%04x copy and inst from %08x %s\n", this, asset_t::addr, &other, typeid(T).name());
            getPtr();
        }
    }

    void operator=(Idx<T, Owner, Addr, Null, Max>&& other) = delete;

    void operator=(const std::nullptr_t&) { dispose(); asset_t::addr = Null; }

    bool operator==(const std::nullptr_t& n) { return asset_t::addr == Null; }
    bool operator==(const Idx<T, Owner, Addr, Null, Max>& other) { return asset_t::addr == other.addr; }

    bool operator!=(const std::nullptr_t& n) { return !(*this == n); }
    bool operator!=(const Idx<T, Owner, Addr, Null, Max>& other) { return !(*this == other); }

private:
    void* getPtr() {
        return asset_t::getPtr(sizeof(T));
    }

    std::size_t& getRefCount() {
        return asset_t::getRefCount(sizeof(T));
    }

    void dispose() {
        rt_kprintf("%08x@%04x disposing %s\n", this, asset_t::addr, typeid(T).name());

        auto count = 0u;

        if(asset_t::isAccessed()) {
            auto ptr = getPtr();
            rt_kprintf("is accessed\n");
            boost::pfr::for_each_field(*(T*)ptr, [&](auto& field, std::size_t idx) {
                if(typeid(field) == typeid(Idx<T, Owner, Addr, Null, Max>)) {
                    if(((Idx<T, Owner, Addr, Null, Max>*)&field)->isAccessed()) {
                        ++count;
                    }
                }
            });
            try {
                auto& refCount = getRefCount();
                rt_kprintf("!!!!!count: %d, ref: %d!!!!!\n", count, getRefCount());
                if(count == refCount - 1 && count != 0) {
                    refCount = 1;
                    rt_kprintf("~~~~~dispose loop~~~~~\n");
                    boost::pfr::for_each_field(*(T*)ptr, [&](auto& field, std::size_t idx) {
                        if(typeid(field) == typeid(Idx<T, Owner, Addr, Null, Max>)) {
                            asset_t::field.erase(&field);
                        }
                    });
                }
            } catch(const std::runtime_error& e) {

            }
        }
        auto obj = asset_t::dispose(sizeof(T));
        if(obj) ((T*)obj.get())->~T();
    }
};

template <class Owner, class Addr, Addr Null, Addr Max>
std::map<void*, typename IdxAsset<Owner, Addr, Null, Max>::Field> IdxAsset<Owner, Addr, Null, Max>::field = {};

template <class Owner, class Addr, Addr Null, Addr Max>
std::map<std::shared_ptr<Owner>, typename IdxAsset<Owner, Addr, Null, Max>::OwnerSpec> IdxAsset<Owner, Addr, Null, Max>::ownerSpecs = {};

template <class Owner, class Addr, Addr Null, Addr Max>
void* IdxAsset<Owner, Addr, Null, Max>::getPtr(std::size_t size) {
    if(field.find(this) == field.end()) {
        field[this] = {};
    }
    auto& f = field[this];
    auto owner = f.owner;
    auto& current = f.current;

    //TODO: 判断是否已求值

    if(addr == Null) throw std::out_of_range{"nullptr"};
    if(addr + size - 1 > Max) throw std::out_of_range{"out of addr space"};
    if(!owner) throw std::runtime_error{"ctx not provided"};

    if(!current) { //未访问过成员
        //1. 判断地址是否已存在, 即判断addr
        auto ownerSpecKvp = ownerSpecs.find(owner);
        if(ownerSpecKvp == ownerSpecs.end()) {
            ownerSpecs[owner] = {};
        }

        auto& ownerSpec = ownerSpecs[owner];
        auto& range = ownerSpec.range;
        auto& count = ownerSpec.count;

        auto dataKvp = ownerSpec.data.find(addr);
        if(dataKvp != ownerSpec.data.end()) {
            auto& data = dataKvp->second;
            //如果找到, 则需要校验, 判断size是否满足要求
            auto foundIt = std::find(range.begin(), range.end(), addr);
            auto dist = std::distance(range.begin(), foundIt);
            auto nextIt = foundIt;
            ++nextIt;
            auto rangeSize = std::size_t(*nextIt - addr);
            if(rangeSize != size) throw std::runtime_error{"range size mismatch"};
            auto countIt = ownerSpec.count.begin();
            std::advance(countIt, dist);
            auto& countVal = *countIt;
            if(countVal <= 0) throw std::runtime_error{"range count error"};
            current = data.current;
            ++countVal;

            rt_kprintf("after +@%02x, range: ", addr);
            printContainer(range);
            rt_kprintf(", count: ");
            printContainer(count);
            rt_kprintf("\n");

            return (void*)current.get();
        }

        //没找到则创建
        //裂开range、count链表
        auto front = std::lower_bound(range.begin(), range.end(), addr);
        auto rightAddr = addr + size;
        if(*front != addr) {
            //需要裂开, 但是得判断与原有的有没有交叉，有的话抛异常

            if(rightAddr > *front) throw not_implemented{"cross detected"};

            //判断是否落在引用计数大于0的范围内
            auto countFront = count.begin();
            std::advance(countFront, std::distance(range.begin(), front) - 1);
            if(*countFront != 0) throw not_implemented{"cross detected"};
            ++countFront;

            if(rightAddr == *front) {
                //右边不需要裂开
                range.insert(front, addr);
                count.insert(countFront, 1);
            } else {
                //右边需要裂开
                range.insert(front, addr);
                count.insert(countFront, 1);
                range.insert(front, rightAddr);
                count.insert(countFront, 0);
            }

        } else {
            //左边不需要裂开, 如果不需要裂开, 且计数 > 0, 需要特殊处理
            auto back = front;
            ++back;
            if(rightAddr > *back) throw not_implemented{"cross detected"};
            auto countback = count.begin();
            std::advance(countback, std::distance(range.begin(), back) - 1);
            if(*countback != 0) throw not_implemented{"cross detected"};

            ++*countback; //自身计数+1

            ++countback;

            if(rightAddr != *back) {
                //右边需要裂开
                range.insert(back, rightAddr);
                count.insert(countback, 0);
            }
        }


        rt_kprintf("after ^@%02x, range: ", addr);
        printContainer(range);
        rt_kprintf(", count: ");
        printContainer(count);
        rt_kprintf("\n");

        auto data = std::shared_ptr<rt_uint8_t[]>(new rt_uint8_t[size]);
        auto backup = std::shared_ptr<rt_uint8_t[]>(new rt_uint8_t[size]);

        owner->read(addr, data.get(), size);
        memcpy(backup.get(), data.get(), size);
        current = data;

        auto& curData = ownerSpec.data[addr];

        curData.current = data;
        curData.backup = backup;
        return (void*)current.get();
    }

    return (void*)current.get();
}

template <class Owner, class Addr, Addr Null, Addr Max>
std::size_t& IdxAsset<Owner, Addr, Null, Max>::getRefCount(std::size_t size) {
    if(field.find(this) == field.end()) {
        field[this] = {};
    }
    auto& f = field[this];
    auto owner = f.owner;

    auto ownerSpecKvp = ownerSpecs.find(owner);
    if(ownerSpecKvp == ownerSpecs.end()) {
        throw std::runtime_error{"not prepared"};
    }

    auto& ownerSpec = ownerSpecs[owner];
    auto& range = ownerSpec.range;

    auto dataKvp = ownerSpec.data.find(addr);
    if(dataKvp == ownerSpec.data.end()) {
        throw std::runtime_error{"not prepared"};
    }

    auto foundIt = std::find(range.begin(), range.end(), addr);
    auto dist = std::distance(range.begin(), foundIt);
    auto nextIt = foundIt;
    ++nextIt;
    auto rangeSize = std::size_t(*nextIt - addr);
    if(rangeSize != size) throw std::runtime_error{"range size mismatch"};
    auto countIt = ownerSpec.count.begin();
    std::advance(countIt, dist);
    auto& countVal = *countIt;
    if(countVal <= 0) throw std::runtime_error{"range count error"};
    return countVal;
}

template <class Owner, class Addr, Addr Null, Addr Max>
bool IdxAsset<Owner, Addr, Null, Max>::isAccessed() const {
    if(field.find((void*)this) == field.end()) {
        return false;
    }
    return field[(void*)this].current != nullptr;
}

template <class Owner, class Addr, Addr Null, Addr Max>
std::shared_ptr<rt_uint8_t[]> IdxAsset<Owner, Addr, Null, Max>::dispose(std::size_t size) {
    if(field.find(this) == field.end()) return nullptr;

    auto& f = field[this];
    auto owner = f.owner;
    auto& current = f.current;

    if(!current) {
        rt_kprintf("%08x current is null\n", this);
        return nullptr;
    }

    rt_kprintf("%08x current is not null\n", this);

    auto& ownerSpec = ownerSpecs[owner];

    //减少引用计数、合并未引用的range
    auto& range = ownerSpec.range;
    auto& count = ownerSpec.count;

    auto found = std::find(range.begin(), range.end(), addr);
    auto countIt = count.begin();
    std::advance(countIt, std::distance(range.begin(), found));
    auto beforeIt = countIt;
    --beforeIt;

    auto afterIt = countIt;
    ++afterIt;

    --*countIt;
    if(*countIt <= 0) {
        //准备合并
        if(*afterIt == 0) {
            //合并后面
            auto afterRangeIt = found;
            ++afterRangeIt;

            rt_kprintf("erase range value %d\n", *afterRangeIt);
            rt_kprintf("erase count @%d\n", std::distance(count.begin(), afterIt));
            range.erase(afterRangeIt);
            count.erase(afterIt);
        }

        if(countIt != count.begin()) {

            if(*beforeIt == 0) {
                auto beforeRangeIt = found;

                rt_kprintf("erase range value %d\n", *beforeRangeIt);
                rt_kprintf("erase count @%d\n", std::distance(count.begin(), countIt));

                range.erase(beforeRangeIt);
                count.erase(countIt);
            }
        }

        //判断是否满足写回条件
        auto& data = ownerSpec.data[addr];
        if(memcmp(data.current.get(), data.backup.get(), size)) {
            owner->write(addr, data.current.get(), size);
        }

        auto result = data.current;
        ownerSpec.data.erase(addr);

        rt_kprintf("after =@%02x, range: ", addr);
        printContainer(range);
        rt_kprintf(", count: ");
        printContainer(count);
        rt_kprintf("\n");

        return result;
    }

    rt_kprintf("after -@%02x, range: ", addr);
    printContainer(range);
    rt_kprintf(", count: ");
    printContainer(count);
    rt_kprintf("\n");

    current = nullptr;
    return nullptr;
}

#endif /* UTILITIES_IDXBASE_HXX_ */
