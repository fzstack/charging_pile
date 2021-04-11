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
#include <ui/loading_widget.hxx>

using namespace std;

User::User(
    std::shared_ptr<UserInput> input,
    std::shared_ptr<RgbNotifierLite> rgb,
    std::shared_ptr<VoiceNotifierLite> voice
) : input(input), rgb(rgb), voice(voice) {
    input->onConfirm += [this](NatPort port, string cardId){
        this->onInputConfirm(port, cardId);
    };

}

void User::boradcast(int balance, BroadcastType type) {
    voice->boradcast(balance, type);
}
