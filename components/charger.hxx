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
#include "virtual_load_detector.hxx"
#include "local_state_store.hxx"
#include "voice_notifier.hxx"
#include <memory>

class Charger {
public:
    Charger(
        std::shared_ptr<Relay> relay,
        std::shared_ptr<Multimeter::Channel> multimeterChannel,
        std::shared_ptr<VirtualLoadDetector> vlodet,
        std::shared_ptr<StateStore> stateStore
    );
public:
    void init();
    void start();
    void stop();

private:
    void assertState(State::Value expectedState, std::string action);


private:
    std::shared_ptr<Relay> relay;

public:
    std::shared_ptr<Multimeter::Channel> multimeterChannel;

private:
    std::shared_ptr<VirtualLoadDetector> vlodet;

public:
    std::shared_ptr<StateStore> stateStore;

private:
    Observable<int> inited = {false};
};

#include <utilities/singleton.hxx>
#include "state_store_preset.hxx"

namespace Preset {
template<int R>
class Charger: public Singleton<Charger<R>>, public ::Charger {
    friend class Singleton<Charger<R>>;
    Charger(): ::Charger(
        Relay<R>::get(),
        MultiMeterChannel<R>::get(),
        VirtualLoadDetector<R>::get(),
        StateStore<R>::get()
    ) {
        MultiMeterChannel<R>::Owner::get()->init();
        VirtualLoadDetector<R>::get()->init();
        //VoiceNotifier::get()->watch(StateStore<R>::get(), getPSV());
    }

    static constexpr PortSpecifiedVoice getPSV() {
        switch(R) {
        case 0:
            return {Voices::Port0Pluged, Voices::Port0Unpluged};
        case 1:
            return {Voices::Port1Pluged, Voices::Port1Unpluged};
        case 2:
            return {Voices::Port2Pluged, Voices::Port2Unpluged};
        case 3:
            return {Voices::Port3Pluged, Voices::Port3Unpluged};
        case 4:
            return {Voices::Port4Pluged, Voices::Port4Unpluged};
        case 5:
            return {Voices::Port5Pluged, Voices::Port5Unpluged};
        case 6:
            return {Voices::Port6Pluged, Voices::Port6Unpluged};
        case 7:
            return {Voices::Port7Pluged, Voices::Port7Unpluged};
        case 8:
            return {Voices::Port8Pluged, Voices::Port8Unpluged};
        case 9:
            return {Voices::Port9Pluged, Voices::Port9Unpluged};
        default:
            return {Voices::Slience, Voices::Slience};
        }
    }
};
}

struct charger_error: public std::runtime_error {
    using std::runtime_error::runtime_error;
};

#endif /* COMPONENTS_CHARGER_HXX_ */
