#pragma once

#include <devices/rc522.hxx>
#include <components/keyboard.hxx>
#include <utilities/signals.hxx>
#include <string>
#include <utilities/nat_port.hxx>
#include <optional>
#include <components/timer.hxx>
#include <utilities/count_down.hxx>

class UserInput {
public:
    UserInput() = default;
    void watch(std::shared_ptr<Rc522Base> rc522);
    void watch(std::shared_ptr<Keyboard> keyboard);

    Signals<void(std::string)> onCardSwipe = {};
    Signals<void(NatPort port, std::string cardId)> onConfirm = {};

    enum class Error {
        PortInvalid,
        CardRequired,
        PortSelectRequired,
    };
    Signals<void(Error)> onError = {};

private:
    Timer timer = {1000, "usript"};
    CountDown<> cardInvalid = {60};
    std::string lastCard;
    std::optional<int> currPort = std::nullopt;
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

