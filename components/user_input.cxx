#include <components/user_input.hxx>
#include <unordered_map>
#include <limits>

using namespace std;

UserInput::UserInput(std::shared_ptr<AppState> appState): appState(appState) {

}

void UserInput::watch(std::shared_ptr<Rc522Base> rc522) {
    rc522->oCardId += [this](auto value) {
        if(!value) return;
        onCardSwipe(*value);
        lastCard = *value;
        cardInvalid.retrigger();
    };

    timer.onRun += [this]{
        if(cardInvalid.updateAndCheck()) {
            lastCard.clear();
        }
    };

    timer.start();
}

void UserInput::watch(std::shared_ptr<Keyboard> keyboard) {
    keyboard->oValue += [this](auto value) {
        if(!value) return;
        switch(*value) {
        case Keyboard::Keys::Ret:
            currPort = std::nullopt;
            break;
        case Keyboard::Keys::Ok: {
            auto adminCond = false;
            if(currPort) {
                auto op = (AdminOp)*currPort;
                if(kKnownAdminOp.count(op)) {
                    adminCond = true;
                    rt_kprintf("admin op\n");
                    onAdminOp(op);
                }
            }

            if(!adminCond) {
                if(lastCard.empty()) {
                    onError(make_exception_ptr<Error>(ErrorCode::CardRequired));
                    break;
                }
                if(!currPort) {
                    onError(make_exception_ptr<Error>(ErrorCode::PortSelectRequired));
                    break;
                }

                auto port = NatPort{(rt_uint8_t)*currPort};
                if(*currPort > numeric_limits<rt_uint8_t>::max() || !port.validate()) {
                    onError(make_exception_ptr<Error>(ErrorCode::PortInvalid));
                } else if(appState->getPortState(port) == State::Charging) {
                    onError(make_exception_ptr<PortInUseError>(port));
                } else {
                    onConfirm(port, lastCard);
                }
            }

            lastCard.clear();
            cardInvalid.reset();
            currPort = std::nullopt;
            break;
        }
        default:
            if(Keyboard::isNumberKey(*value)) {
                auto num = Keyboard::keyToNumber(*value);
                if(currPort == std::nullopt) {
                    currPort = 0;
                }
                if(*currPort < 9999) {
                    *currPort *= 10;
                    *currPort += num;
                }
            }
            break;
        }
    };
}

const std::set<UserInput::AdminOp> UserInput::kKnownAdminOp = {AdminOp::ClearConf};
