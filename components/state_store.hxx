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
#include "virtual_load_detector.hxx"
#include "timer.hxx"

class StateStore: public StateStoreBase {
    using StateStoreBase::StateStoreBase;
public:
    StateStore();
    void watch(std::shared_ptr<Relay> relay);
    void watch(std::shared_ptr<Multimeter::Channel> multimeterChannel);
    void watch(std::shared_ptr<VirtualLoadDetector> virtualLoadDetector);
private:
    void update();

private:
    std::shared_ptr<Relay> relay;
    std::shared_ptr<Multimeter::Channel> multimeterChannel;
    std::shared_ptr<VirtualLoadDetector> virtualLoadDetector;

private:
    rtthread::Mutex mutex;
    static const char* kMutex;

};

#include <utilities/singleton.hxx>
namespace Preset {
template<int R>
class StateStore: public Singleton<StateStore<R>>, public ::StateStore {
    friend class Singleton<StateStore<R>>;
    StateStore(): ::StateStore() {}
};
}



#endif /* COMPONENTS_STATE_STORE_HXX_ */
