/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-03-26     imgcr       the first version
 */

#include "user.hxx"

using namespace std;

User::User(
    std::shared_ptr<UserInput> input,
    std::array<std::shared_ptr<RgbStateNotifier>, Config::Bsp::kPortNum>&& rgbs,
    std::shared_ptr<VoiceNotifier> voice
) : input(input), rgbs(rgbs), voice(voice) {
    input->onConfirm += [this](NatPort port, string cardId){
        this->onInputConfirm(port, cardId);
    };
}

