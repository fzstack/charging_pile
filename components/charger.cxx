/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-12-30     imgcr       the first version
 */

#include "charger.hxx"

Charger::Charger(
    std::shared_ptr<RgbStateNotifier> rgbNotifier,
    std::shared_ptr<Relay> relay,
    std::shared_ptr<Multimeter::Channel> multimeterChannel,
    std::shared_ptr<VirtualLoadDetector> vlodet,
    std::shared_ptr<StateStore> stateStore
): rgbNotifier(rgbNotifier), relay(relay), multimeterChannel(multimeterChannel), vlodet(vlodet), stateStore(stateStore) {
    inited.onChanged += [this](auto value) {
        if(!value) return;
        this->rgbNotifier->watch(this->stateStore);
        this->stateStore->watch(this->relay);
        this->stateStore->watch(this->multimeterChannel);
        this->stateStore->watch(this->vlodet);
    };
};

void Charger::init() {
    inited = true;
}

void Charger::start() {
    //TODO: 判断是否可以充电
    relay->getHandler() = true;
}

void Charger::stop() {
    relay->getHandler() = false;
}
