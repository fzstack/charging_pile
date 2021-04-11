#include <components/user_input.hxx>
#include <unordered_map>
#include <limits>

using namespace std;

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
            if(lastCard.empty()) {
                onError(Error::CardRequired);
                break;
            }
            if(!currPort) {
                onError(Error::PortSelectRequired);
                break;
            }
            auto port = NatPort{(rt_uint8_t)*currPort};
            if(*currPort > numeric_limits<rt_uint8_t>::max() || !port.validate()) {
                onError(Error::PortInvalid);
            } else {
                onConfirm(port, lastCard);
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
