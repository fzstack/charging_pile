/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-12-30     imgcr       the first version
 */
#ifndef COMPONENTS_STATE_STORE_HXX_
#define COMPONENTS_STATE_STORE_HXX_

#include "state_store_base.hxx"
#include <devices/relay.hxx>
#include "multimeter.hxx"
#include <devices/load_detector.hxx>

class StateStore: public StateStoreBase {
    using StateStoreBase::StateStoreBase;
public:
    void watch(std::shared_ptr<Relay> relay);
    void watch(std::shared_ptr<Multimeter::Channel> multimeterChannel);
    void watch(std::shared_ptr<LoadDetector> loadDetector);
private:
    void update();
};



#endif /* COMPONENTS_STATE_STORE_HXX_ */
