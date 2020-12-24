/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-08-26     imgcr       the first version
 */

#include "rgb_state_notifier.hxx"
#include <utilities/lock_guard.hxx>
#include <map>
#include <string>
#include <functional>

#define LOG_TAG "dev.rgb"
#define LOG_LVL LOG_LVL_DBG
#include <ulog.h>

RgbStateNotifier::RgbStateNotifier(std::shared_ptr<RgbLight> light): light(light) {
    mutex = std::shared_ptr<rt_mutex>(rt_mutex_create("RSN", RT_IPC_FLAG_FIFO), [](auto p) {
        rt_mutex_delete(p);
    });

    k.onChanged += [this](auto value) {
        render();
    };

    timer = std::shared_ptr<rt_timer>(rt_timer_create("RSN", [](auto p) {
        auto self = (RgbStateNotifier*)p;
        self->k++;
    }, this, kRoundDurMs, RT_TIMER_FLAG_PERIODIC | RT_TIMER_FLAG_SOFT_TIMER), [](auto p) {
        rt_timer_stop(p);
        rt_timer_delete(p);
    });
    rt_timer_start(timer.get());
}

void RgbStateNotifier::watch(std::shared_ptr<StateStoreBase> store) {
    this->store = store;
    auto self(shared_from_this());
    store->oState += [this, self](auto next) {
        render();
    };
}

void RgbStateNotifier::render() {
    auto guard = LockGuard(mutex.get());

    if(!store || !store->oState.value()) {
        light->r = light->g = light->b = false;
        return;
    }

    switch(*store->oState.value()) {
        case State::LoadNotInsert:
            light->g = true;
            light->r = light->b = false;
            break;
        case State::LoadInserted:
            light->r = *k % 2;
            light->g = light->b = false;
            break;
        case State::Charging:
            light->g = *k % 2;
            light->r = light->b = false;
            break;
        case State::LoadWaitRemove:
            light->b = true;
            light->r = light->g = false;
            break;
        case State::Error:
            light->g =  *k % 2;
            light->r = !(*k % 2);
            light->b = false;
            break;
    }
}
