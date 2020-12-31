/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-12-30     imgcr       the first version
 */
#ifndef COMPONENTS_CHARGER_HXX_
#define COMPONENTS_CHARGER_HXX_

#include "rgb_state_notifier.hxx"
#include <devices/relay.hxx>
#include "multimeter.hxx"
#include <devices/load_detector.hxx>
#include "state_store.hxx"
#include <memory>

class Charger {
public:
    Charger(
        std::shared_ptr<RgbStateNotifier> rgbNotifier,
        std::shared_ptr<Relay> relay,
        std::shared_ptr<Multimeter::Channel> multimeterChannel,
        std::shared_ptr<LoadDetector> loadDetector,
        std::shared_ptr<StateStore> stateStore
    );
public:
    void init();

private:
    std::shared_ptr<RgbStateNotifier> rgbNotifier;
    std::shared_ptr<Relay> relay;
    std::shared_ptr<Multimeter::Channel> multimeterChannel;
    std::shared_ptr<LoadDetector> loadDetector;
    std::shared_ptr<StateStore> stateStore;

    Observable<int> inited = {false};
};



#endif /* COMPONENTS_CHARGER_HXX_ */
