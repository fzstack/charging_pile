/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-03-30     imgcr       the first version
 */
#pragma once
#include <optional>
#include <memory>
#include <utilities/count_down.hxx>
#include <components/persistent_storage_preset.hxx>
#include <Mutex.h>

//TODO: RPC超时
template<class T>
class ConfMan {
public:
    ConfMan(rtthread::Mutex& mutex): mutex(mutex) {
        mutex.lock();
        flag.trigger();
        mutex.unlock();
    }

    void refresh() { //请在定时器内调用
        mutex.lock();
        auto result = flag.updateAndCheck();
        mutex.unlock();
        if(result) {
            prepare();
        }
    }

    const std::optional<T>& get() {
        return params;
    }

    bool operator ==(std::nullopt_t) {
        return params == std::nullopt;
    }

    bool operator !=(std::nullopt_t) {
        return !(*this == std::nullopt);
    }

    T* operator->() {
        return params.operator ->();
    }

    void save(T&& t) {
        mutex.lock();
        params = t;
        mutex.unlock();
        Preset::PersistentStorage::get()->make<T>([=](auto p) {
            *p = t;
        });
    }

private:
    void prepare() {
        Preset::PersistentStorage::get()->make<T>([=](auto p){
            if(p == nullptr) {
                mutex.lock();
                flag.retrigger();
                mutex.unlock();
                return;
            }
            params = *p;
        });
    }

private:
    rtthread::Mutex& mutex;
    std::optional<T> params;
    CountDown<> flag = {};
};
