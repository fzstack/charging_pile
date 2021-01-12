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

    template <class T>
    class Idx { //NOTE: 改变指针要调用dispose
    public:
        Idx(rt_uint16_t addr): addr(addr) {
            field[this] = {};
        }

        Idx(): addr(kNull) { //空Idx
            field[this] = {};
        }

        ~Idx() {
            dispose();
            field.erase(this);
        }

        T* operator->() {
            return &getRef();
        }

        T& operator*() {
            return getRef();
        }

        void operator=(rt_uint16_t addr) {
            dispose();
            this->addr = addr;
        }

        void operator=(const Idx<T>& other) {
            dispose();
            addr = other.addr;
        }

        void operator=(nullptr_t) {
            dispose();
            addr = kNull;
        }

        bool operator==(const nullptr_t& n) {
            return addr == kNull;
        }

        bool operator==(const Idx<T>& other) {
            return addr == other.addr;
        }

        T& getRef() { //TODO: 判断地址是否有效
            auto& f = field[this];
            auto outer = f.outer;
            auto& info = f.info;
            if(info == nullptr) { //尚未求值
                if(!outer) throw std::runtime_error{"ctx not provided"};
                auto found_outer = pool.find(outer);
                if(found_outer != pool.end()) {
                    auto found_addr = found_outer->second.find(addr);
                    if(found_addr != found_outer->second.end()) {
                        //找到了，直接返回
                        info = &found_addr->second;
                        info->count++;
                        return info->data;
                    }
                }
                //没找到、创建
                auto& infoRef = pool[outer][addr];
                info = &infoRef;
                info->count = 1;

                at24cxx_read(self->device, addr, (uint8_t*)(void*)&info->data, sizeof(T));
                info->backup = info->data;
                rt_kprintf("load %s from eeprom\n", typeid(T).name());
            }
            return info->data;
        }

        void dispose() {
            auto& f = field[this];
            auto& info = f.info;
            auto outer = f.outer;
            if(info != nullptr) {
                info->count--;
                if(info->count > 0) return;
                if(info->data != info->backup) {
                    rt_kprintf("store %s to eeprom\n", typeid(T).name());
                    at24cxx_write(self->device, addr, (uint8_t*)(void*)&info->data, sizeof(T));
                }
                auto found_outer = pool.find(outer);
                found_outer->second.erase(addr);
            }
        }
        static constexpr rt_uint16_t kNull = 0xffff;
    private:
        rt_uint16_t addr;

        struct IdxInfo {
            T data, backup;
            int count;
        };

        struct IdxField {
            IdxInfo* info = nullptr;
            std::shared_ptr<PersistentStorage> outer = *PersistentStorage::self;
        };

        static std::map<Idx*, IdxField> field;
        static std::map<std::shared_ptr<PersistentStorage>, std::map<rt_uint16_t, IdxInfo>> pool;
    };

    template <class T>
    struct Trivial {
        bool operator==(const T& other) {
            return !memcmp(this, &other, sizeof(T));
        }

        bool operator!=(const T& other) {
            return !(*this == other);
        }
    };

    struct IdleNode: public Trivial<IdleNode> {
        rt_uint16_t size;
        Idx<IdleNode> prev;
        Idx<IdleNode> next;
    };

    struct AllocNode: public Trivial<AllocNode> {
        rt_uint16_t size;
        Idx<AllocNode> prev;
        Idx<AllocNode> next;
    };

    struct MetaNode: public Trivial<MetaNode> { //元数据节点
        std::size_t hash; //类型哈希
        //TODO: field链表首元节点
        Idx<MetaNode> next;
    };

    struct MetaHead: public Trivial<MetaHead> { //元数据头, 放在eeprom开头
        std::size_t magic; //魔数，判断是否需要格式化
        Idx<IdleNode> idle; //空闲链表首元
        Idx<AllocNode> alloc; //元数据链表首元

        static Idx<MetaHead> get() {
            return Idx<MetaHead>(0);
        }
    };


public:

//    template<class T>
//    std::shared_ptr<T> make() {
//        //返回指定类型的智能指针
//    }

    void format();
    void alloc();

private:
    std::shared_ptr<void> getCtxGuard();

private:
    at24cxx_device_t device;
    const int size;
    static ThreadLocal<std::shared_ptr<PersistentStorage>> self;
};

template<class T>
std::map<std::shared_ptr<PersistentStorage>, std::map<rt_uint16_t, typename PersistentStorage::Idx<T>::IdxInfo>> PersistentStorage::Idx<T>::pool = {};

template<class T>
std::map<typename PersistentStorage::Idx<T>*, typename PersistentStorage::Idx<T>::IdxField> PersistentStorage::Idx<T>::field = {};


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
