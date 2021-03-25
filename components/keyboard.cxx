#include "keyboard.hxx"

using namespace std;

Keyboard::Keyboard(std::shared_ptr<Ttp229> device): device(device) {
    device->onData += [this](auto data) {
        if(data == 0) {
            value = nullopt;
        } else {
            if(values.count(Keys(data))) {
                value = Keys(data);
            }
        }
    };
}

const std::set<Keyboard::Keys> Keyboard::values = {
    Keys::K1, Keys::K2, Keys::K3,
    Keys::K4, Keys::K5, Keys::K6,
    Keys::K7,  Keys::K8,  Keys::K9,
    Keys::Ret, Keys::K0,  Keys::Ok,
};
