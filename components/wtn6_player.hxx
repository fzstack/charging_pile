/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-04-09     imgcr       the first version
 */
#pragma once
#include "player.hxx"
#include <devices/wtn6.hxx>

class Wtn6Player: public Player {
public:
    Wtn6Player(std::shared_ptr<Wtn6> wtn6);
protected:
    virtual void playFragment(VoiceFragment fragment) override;
    virtual bool isFragmentPlaying() override;
private:
    std::shared_ptr<Wtn6> wtn6;
};

#include <utilities/singleton.hxx>
namespace Preset {
class Wtn6Player: public Singleton<Wtn6Player>, public ::Wtn6Player {
    friend class Singleton<Wtn6Player>;
    Wtn6Player(): ::Wtn6Player(Wtn6::get()) {}
};
}

