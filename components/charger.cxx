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
    std::shared_ptr<LoadDetector> loadDetector,
    std::shared_ptr<StateStore> stateStore
): rgbNotifier(rgbNotifier), relay(relay), multimeterChannel(multimeterChannel), loadDetector(loadDetector), stateStore(stateStore) {
    inited.onChanged += [this](auto value) {
        //rgb  watch  stateNotifier

        //this->rgbNotifier->watch(stateStore);

        //stateNotifier watch relay
        //stateNotifier watch loadDetector
        //stateNotifier watch multimeterChannel
    };
};

void Charger::init() {
    inited = true;
}

