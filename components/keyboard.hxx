#pragma once

#include <devices/ttp229.hxx>
#include <memory>
#include <utilities/observable.hxx>
#include <utilities/observer.hxx>
#include <optional>
#include <set>
#include <utilities/count_down.hxx>
#include <components/timer.hxx>

class Keyboard {
public:
    Keyboard(std::shared_ptr<Ttp229> device);

    enum class Keys {
        K1 = 15, K2 = 14, K3 = 13,
        K4 = 10, K5 = 11, K6 = 12,
        K7 = 9,  K8 = 4,  K9 = 5,
        Ret = 6, K0 = 7,  Ok = 8,
    };

private:
    Observable<std::optional<Keys>> value = {std::nullopt};
public:
    Observer<std::optional<Keys>> oValue = value;

private:
    Timer timer = {100, "kbd"};
    rt_uint16_t data;
    CountDown<> fKey = {};
    std::shared_ptr<Ttp229> device;
    static const std::set<Keys> values;
};

#include <utilities/singleton.hxx>
namespace Preset {
class Keyboard: public Singleton<Keyboard>, public ::Keyboard {
    friend singleton_t;
    Keyboard(): ::Keyboard(Ttp229::get()) {}
};
}
