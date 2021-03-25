#pragma once

#include <devices/rc522.hxx>
#include <components/keyboard.hxx>
#include <utilities/signals.hxx>
#include <string>
#include <utilities/nat_port.hxx>

class UserInput {
public:
    UserInput() = default;
    void watch(std::shared_ptr<Rc522Base> rc522);
    void watch(std::shared_ptr<Keyboard> keyboard);

    Signals<void(std::string)> onCardSwipe = {};
    Signals<void(NatPort port, std::string cardId)> onConfirm = {};

private:
    std::string lastCard;
    int currPort = 0;
};

#include <utilities/singleton.hxx>
namespace Preset {
class UserInput: public Singleton<UserInput>, public ::UserInput {
    friend singleton_t;
    UserInput(): ::UserInput() {
        watch(Rc522::get());
        watch(Keyboard::get());
    }
};
}

