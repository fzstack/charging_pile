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
#include <components/rgb_notifier_lite.hxx>
#include <components/voice_notifier_lite.hxx>
#include <components/user_input.hxx>
#include <memory>
#include <config/bsp.hxx>
#include <utilities/signals.hxx>
#include <string>
#include <array>
#include <utilities/mp.hxx>
#include <ui/present.hxx>

class User {
public:
    User(std::shared_ptr<UserInput> input, std::shared_ptr<RgbNotifierLite> rgb, std::shared_ptr<VoiceNotifierLite> voice);

    Signals<void(NatPort port, std::string cardId)> onInputConfirm;

private:
    std::shared_ptr<UserInput> input;
    std::shared_ptr<RgbNotifierLite> rgb;
    std::shared_ptr<VoiceNotifierLite> voice;
};

#include <utilities/singleton.hxx>
namespace Preset {
class User: public Singleton<User>, public ::User {
    friend class Singleton<User>;
    User(): ::User(UserInput::get(), nullptr/*RgbNotifierLite::get()*/, VoiceNotifierLite::get()) {
        Present::get();
    }
};
}
