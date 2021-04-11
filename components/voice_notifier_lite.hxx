/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-04-01     imgcr       the first version
 */
#pragma once
#include <components/player_preset.hxx>
#include <components/app_state.hxx>
#include <components/user_input.hxx>
#include <array>
#include <components/keyboard.hxx>
#include "voice.hxx"

class VoiceNotifierLite {
public:
    VoiceNotifierLite(std::shared_ptr<Player> player, std::shared_ptr<AppState> state, std::shared_ptr<UserInput> userInput, std::shared_ptr<Keyboard> keybaord);
    void boradcast(int balance, BroadcastType type);
private:
    std::shared_ptr<Player> player;
};

#include <utilities/singleton.hxx>
namespace Preset {
class VoiceNotifierLite: public Singleton<VoiceNotifierLite>, public ::VoiceNotifierLite {
    friend singleton_t;
    VoiceNotifierLite(): ::VoiceNotifierLite(Player::get(), AppState::get(), UserInput::get(), Keyboard::get()) { }
};
}
