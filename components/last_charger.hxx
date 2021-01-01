/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-01-01     imgcr       the first version
 */
#ifndef COMPONENTS_LAST_CHARGER_CXX_
#define COMPONENTS_LAST_CHARGER_CXX_

#include "state_store_base.hxx"
#include <config/bsp.hxx>
#include <array>
#include <memory>
#include <utilities/observable.hxx>
#include <utilities/observer.hxx>
#include <optional>

class LastCharger {
public:
    LastCharger();
    void watch(std::shared_ptr<StateStoreBase> store);

private:
    Observable<std::optional<std::shared_ptr<StateStoreBase>>> waitingToStart = {std::nullopt};
public:
    Observer<std::optional<std::shared_ptr<StateStoreBase>>> oWaitingToStart;

};

#include <utilities/singleton.hxx>
#include "state_store.hxx"
namespace Preset {
class LastCharger: public Singleton<LastCharger>, public ::LastCharger {
    friend class Singleton<LastCharger>;
    LastCharger(): ::LastCharger() {
        watch(StateStore<0>::get());
        watch(StateStore<1>::get());
    }
};
}


#endif /* COMPONENTS_LAST_CHARGER_CXX_ */
