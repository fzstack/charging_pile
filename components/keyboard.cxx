#include "keyboard.hxx"
#include <stdexcept>

using namespace std;

Keyboard::Keyboard(std::shared_ptr<Ttp229> device): device(device) {
    device->onData += [this](auto d) {
        currData = d;
    };

    timer.onRun += [this] {
        i++;
        i %= data.size();
        //0 .. 15
        data[i] = currData;

        //取众数
        for(auto d: data) {
            ++hist[d];
        }
        auto mode = std::max_element(hist.begin(), hist.end()) - hist.begin();
        auto cnt = hist[mode];
        hist.fill(0);
        if(cnt < validThr || rt_tick_get() < 2000) return;
        if(mode == 0) {
            value = nullopt;
        } else {
            if(values.count(Keys(mode))) {
                value = Keys(mode);
            }
        }
    };

    timer.start();
}

int Keyboard::keyToNumber(Keys key) {
    auto found = keyMap.find(key);
    if(found == keyMap.end()) {
        throw invalid_argument{"not a number key"};
    }
    return found->second;
}

bool Keyboard::isNumberKey(Keys key) {
    auto found = keyMap.find(key);
    return found != keyMap.end();
}

const std::set<Keyboard::Keys> Keyboard::values = {
    Keys::K1, Keys::K2, Keys::K3,
    Keys::K4, Keys::K5, Keys::K6,
    Keys::K7,  Keys::K8,  Keys::K9,
    Keys::Ret, Keys::K0,  Keys::Ok,
};

const std::unordered_map<Keyboard::Keys, int> Keyboard::keyMap = {
    {Keys::K0, 0}, {Keys::K1, 1}, {Keys::K2, 2}, {Keys::K3, 3}, {Keys::K4, 4},
    {Keys::K5, 5}, {Keys::K6, 6}, {Keys::K7, 7}, {Keys::K8, 8}, {Keys::K9, 9},
};
