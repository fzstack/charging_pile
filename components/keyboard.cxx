#include "keyboard.hxx"

using namespace std;

Keyboard::Keyboard(std::shared_ptr<Ttp229> device): device(device) {
    device->onData += [this](auto d) {
        data = d;
        //rt_kprintf("K sent: %d\n", data);
        fKey.retrigger();
    };

    timer.onRun += [this] {
        if(fKey.updateAndCheck()) {
            //rt_kprintf("K recv: %d\n", data);
            if(data == 0) {
                value = nullopt;
            } else {
                if(values.count(Keys(data))) {
                    value = Keys(data);
                }
            }
        }
    };

    timer.start();
}

const std::set<Keyboard::Keys> Keyboard::values = {
    Keys::K1, Keys::K2, Keys::K3,
    Keys::K4, Keys::K5, Keys::K6,
    Keys::K7,  Keys::K8,  Keys::K9,
    Keys::Ret, Keys::K0,  Keys::Ok,
};
