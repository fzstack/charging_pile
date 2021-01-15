/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-01-04     imgcr       the first version
 */
#ifndef COMPONENTS_PERSISTENT_STORAGE_HXX_
#define COMPONENTS_PERSISTENT_STORAGE_HXX_

#include <memory>
#include <rtthread.h>
#include <utilities/thread_local.hxx>
#include <stdexcept>
#include <utilities/idx.hxx>

extern "C" {
#include <at24cxx.h>
}

//用法: 使用make返回智能指针，然后自己存储对象的弱指针
//所有智能指针都指向内存的同一个对象，当一个指针指针引用计数归零时，在e²prom中保存数据

//比如
//auto config = ps->make<Config>();
//NOTE: 如果e²prom不存在对应的对象的数据，则调用对象的默认构造器（比如默认参数就可以这样实现）
//config->cnt++;
//然后config的智能指针被析构之后，把数据保存到e²prom中

class PersistentStorage: public std::enable_shared_from_this<PersistentStorage> {
    template <class T>
    class IdxSess;
    template <class T> friend class IdxSess;
public:
    PersistentStorage(at24cxx_device_t device, int size);

private:

    class IdxOwner {
    public:
        IdxOwner(std::shared_ptr<PersistentStorage> outer): outer(outer) {

        }

        void read(rt_uint16_t addr, rt_uint8_t* data, std::size_t size) {
            //at24cxx_read(outer->device, addr, data, size);
            memcpy(data, &buffer[addr], size);
            rt_kprintf("\033[32mread @%04x, size %d: ", addr, size);
            for(auto i = 0u; i < size; i++) {
                rt_kprintf("%02x ", data[i]);
            }
            rt_kprintf("\n\033[0m");
        }

        void write(rt_uint16_t addr, rt_uint8_t* data, std::size_t size) {
            rt_kprintf("\033[34mwrite @%04x, size %d: ", addr, size);
            for(auto i = 0u; i < size; i++) {
                rt_kprintf("%02x ", data[i]);
            }
            rt_kprintf("\n\033[0m");
            //at24cxx_write(outer->device, addr, data, size);
            memcpy(&buffer[addr], data, size);
        }

        //TODO: 使用弱指针
        static std::shared_ptr<IdxOwner> get() {
            return *owner;
        }

        static ThreadLocal<std::shared_ptr<IdxOwner>> owner;

    private:
        std::shared_ptr<PersistentStorage> outer;
        std::array<rt_uint8_t, 2048> buffer;
    };

    friend class IdxOwner;

    template<class T>
    using Idx = ::Idx<T, IdxOwner, rt_uint16_t, 0xffff, 2048 - 1>;


    struct HeapNode {
        HeapNode(rt_uint16_t size): size(size) {

        }

        rt_uint16_t size;
        Idx<HeapNode> prev = nullptr;
        Idx<HeapNode> next = nullptr;

        ~HeapNode() {
            rt_kprintf("\033[33midleNode dector\n\033[0m");
        }
    };

    struct HeapList {
        void add(Idx<HeapNode> node);
        void remove(Idx<HeapNode> node);
        Idx<HeapNode> getFront();

    private:
        Idx<HeapNode> front = nullptr;
    };

    struct MetaNode{ //元数据节点
        std::size_t hash; //类型哈希
        //TODO: field链表首元节点
        Idx<MetaNode> next;
    };

    struct Meta { //元数据头, 放在eeprom开头
        Meta(std::size_t deviceSize);
        static Idx<Meta> get();
        static Idx<Meta> create(std::size_t deviceSize);

        std::size_t magic = typeid(Meta).hash_code(); //魔数，判断是否需要格式化
        HeapList idle = {};
        HeapList alloc = {};
    };


public:

//    template<class T>
//    std::shared_ptr<T> make() {
//        //返回指定类型的智能指针
//    }

    void format();
    void test();


private:
    rt_uint16_t alloc(std::size_t size);
    void free(rt_uint16_t addr);

    std::shared_ptr<void> getCtxGuard();

private:
    std::shared_ptr<IdxOwner> owner = nullptr;
    at24cxx_device_t device;
    const int size;
};

#include <utilities/singleton.hxx>
namespace Preset {
class PersistentStorage: public Singleton<PersistentStorage>, public ::PersistentStorage {
    friend class Singleton<PersistentStorage>;
    PersistentStorage(): ::PersistentStorage(at24cxx_init(kI2c, kAddr), kSize) {}
    static const char* kI2c;
    static const int kAddr;
    static const int kSize;
};
}


#endif /* COMPONENTS_PERSISTENT_STORAGE_HXX_ */
