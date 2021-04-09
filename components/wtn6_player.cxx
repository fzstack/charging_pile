/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-04-09     imgcr       the first version
 */

#include "wtn6_player.hxx"

Wtn6Player::Wtn6Player(std::shared_ptr<Wtn6> wtn6): wtn6(wtn6) {

}

void Wtn6Player::play(VoiceFragment fragment) {
    wtn6->write((rt_uint8_t)fragment);
}

bool Wtn6Player::isFragmentPlaying() {
    return wtn6->isBusy();
}
