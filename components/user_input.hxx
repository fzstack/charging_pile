#pragma once

#include <devices/rc522.hxx>
#include <components/keyboard.hxx>
#include <utilities/signals.hxx>
#include <string>
#include <utilities/nat_port.hxx>
#include <optional>
#include <components/timer.hxx>
#include <utilities/count_down.hxx>
#include <components/app_state.hxx>

class UserInput {
public:
    UserInput(std::shared_ptr<AppState> appState);
    void watch(std::shared_ptr<Rc522Base> rc522);
    void watch(std::shared_ptr<Keyboard> keyboard);

    Signals<void(std::string)> onCardSwipe = {};
    Signals<void(NatPort port, std::string cardId)> onConfirm = {};

    enum class AdminOp {
        ClearConf = 9527,
    };
    Signals<void(AdminOp)> onAdminOp = {};

    enum class ErrorCode {
        PortInUse,
        PortInvalid,
        CardRequired,
        PortSelectRequired,
    };

    class Error: public std::runtime_error {
    public:
        Error(ErrorCode code): std::runtime_error(""), code(code) {}

        const ErrorCode code;
    };

    class PortInUseError: public Error {
    public:
        PortInUseError(NatPort port): Error(ErrorCode::PortInUse), port(port) {}
        const NatPort port;
    };

    class PortInvalidError: public Error {
    public:
        PortInvalidError(NatPort port): Error(ErrorCode::PortInvalid), port(port) {}
        const NatPort port;
    };

    Signals<void(std::exception_ptr)> onError = {};

private:
    std::shared_ptr<AppState> appState;
    Timer timer = {1000, "usript"};
    CountDown<> cardInvalid = {60};
    std::string lastCard;
    std::optional<int> currPort = std::nullopt;
    static const std::set<AdminOp> kKnownAdminOp;
};

#include <utilities/singleton.hxx>
namespace Preset {
class UserInput: public Singleton<UserInput>, public ::UserInput {
    friend singleton_t;
    UserInput(): ::UserInput(AppState::get()) {
        watch(Rc522::get());
        watch(Keyboard::get());
    }
};
}

