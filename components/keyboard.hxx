/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-02-04     imgcr       the first version
 */
#pragma once

#include <devices/ttp229.hxx>
#include <memory>
#include <utilities/observable.hxx>
#include <utilities/observer.hxx>
#include <optional>
#include <set>

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
