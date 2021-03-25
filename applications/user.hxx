/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-03-26     imgcr       the first version
 */
#pragma once
#include <components/rgb_state_notifier.hxx>
#include <components/voice_notifier.hxx>
#include <components/user_input.hxx>
#include <memory>
#include <config/bsp.hxx>
#include <utilities/signals.hxx>
#include <string>
#include <array>
#include <utilities/mp.hxx>

class User {
public:
    User(std::shared_ptr<UserInput> input, std::array<std::shared_ptr<RgbStateNotifier>, Config::Bsp::kPortNum>&& rgbs, std::shared_ptr<VoiceNotifier> voice);

    Signals<void(NatPort port, std::string cardId)> onInputConfirm;

private:
    std::shared_ptr<UserInput> input;
    std::array<std::shared_ptr<RgbStateNotifier>, Config::Bsp::kPortNum> rgbs;
    std::shared_ptr<VoiceNotifier> voice;
};

#include <utilities/singleton.hxx>
namespace Preset {
class User: public Singleton<User>, public ::User {
    friend class Singleton<User>;
    User(): ::User(UserInput::get(), std::move(getRgbs()), VoiceNotifier::get()) {

    }

    std::array<std::shared_ptr<::RgbStateNotifier>, Config::Bsp::kPortNum> getRgbs() {
        auto result = std::array<std::shared_ptr<::RgbStateNotifier>, Config::Bsp::kPortNum>{};
        for(int i = 0; i < Config::Bsp::kPortNum; i++) {
            magic_switch<Config::Bsp::kPortNum>{}([&](auto v) {
                result[i] = Preset::RgbStateNotifier<decltype(v)::value>::get();
            }, i);
        }
        return result;
    }
};
}
