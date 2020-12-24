/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-08-26     imgcr       the first version
 */
#ifndef APPLICATIONS2_RGB_STATE_NOTIFIER_HXX_
#define APPLICATIONS2_RGB_STATE_NOTIFIER_HXX_

#include <devices/rgb_light.hxx>
#include "state_store_base.hxx"
#include <memory>
#include <rtthread.h>

class StateStore;

class RgbStateNotifier: public std::enable_shared_from_this<RgbStateNotifier> {
public:
    RgbStateNotifier(std::shared_ptr<RgbLight> light);

    void watch(std::shared_ptr<StateStoreBase> store);

private:
    void render();
    std::shared_ptr<RgbLight> light;
    std::shared_ptr<StateStoreBase> store;
    std::shared_ptr<rt_timer> timer;
    std::shared_ptr<rt_mutex> mutex;
    Observable<int> k;
    static const int kRoundDurMs = 500;
};

#endif /* APPLICATIONS2_RGB_STATE_NOTIFIER_HXX_ */
