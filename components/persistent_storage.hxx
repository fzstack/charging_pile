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

class PersistentStorage {
public:
    PersistentStorage(at24cxx_device_t device);

private:
    using MetaNodeIdx = rt_uint32_t;
    using IdleNodeIdx = rt_uint32_t;
    struct MetaHead { //元数据头, 放在eeprom开头
        std::size_t magic; //魔数
        IdleNodeIdx idle;
        MetaNodeIdx meta;
    };

    struct MetaNode { //元数据节点
        std::size_t hash; //类型哈希
        std::size_t size;
        //TODO: field链表首元节点
        MetaNodeIdx next;
    };

    struct IdleNode {

    };


public:

    template<class T>
    std::shared_ptr<T> make() {
        //返回指定类型的智能指针
    }

private:
    void format();

private:
    at24cxx_device_t device;
};



#endif /* COMPONENTS_PERSISTENT_STORAGE_HXX_ */
