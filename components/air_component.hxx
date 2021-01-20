/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-12-16     imgcr       the first version
 */
#ifndef COMPONENTS_AIR_COMPONENT_HXX_
#define COMPONENTS_AIR_COMPONENT_HXX_

#include "air_component_base.hxx"
#include <map>
#include <vector>

#include <utilities/observable.hxx>
#include <at.h>
#include <functional>
#include <string>
#include <map>

//TODO: 持有本身的弱引用,
class Air724;

template<class T>
class AirComponent: public AirComponentBase {
protected:
    using AirComponentBase::AirComponentBase;

    virtual void init() override {
        auto found = states.find(owner);
        auto state = std::shared_ptr<AirComponentBase::State>(nullptr);
        if(found == states.end()) {
            states.insert({owner, std::make_shared<AirComponentBase::State>()});
            state = states[owner];
            initState(state);
        } else {
            state = found->second;
        }

        if(!state->inst.expired())
            throw std::runtime_error{typeid(T).name() + std::string(" should have only one instance at the same time")};
        state->inst = shared_from_this();
    }

    static std::shared_ptr<T> urc(at_client_t client) {
        for(auto const& kvp: states) {
            if(kvp.first.get() == client->device->user_data) {
                return std::dynamic_pointer_cast<T>(kvp.second->inst.lock());
            }
        }
        return {};
    }

private:
    static std::unordered_map<std::shared_ptr<Air724>, std::shared_ptr<AirComponentBase::State>> states;
};

template<class T> typename std::unordered_map<std::shared_ptr<Air724>, std::shared_ptr<AirComponentBase::State>> AirComponent<T>::states = {};


#endif /* COMPONENTS_AIR_COMPONENT_HXX_ */
