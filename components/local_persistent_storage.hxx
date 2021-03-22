/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-03-21     imgcr       the first version
 */
#pragma once

#include "fal_persistent_storage.hxx"
#include <memory>

//代理模式
class LocalPersistentStorage {
public:
    LocalPersistentStorage(std::shared_ptr<FalPersistentStorage> storage);

    template<class T>
    std::shared_ptr<T> make() {
        //析构的时候写回flash
        //rpc内部还要持有未析构的对象的引用

        //1.RPC返回智能指针，此时RPC框架负责持有智能指针，直到返回值被RAII，此时发送指针被删除packet
    }

private:
    std::shared_ptr<FalPersistentStorage> storage;
};


