/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-01-30     imgcr       the first version
 */
#pragma once

#include <rtthread.h>
#include <utilities/math.hxx>
#include <limits>

namespace Colors {
struct Rgb {
    rt_uint8_t r, g, b;
};

struct Argb {
    rt_uint8_t a, r, g, b;

    //后者为前景
    Argb operator+(const Argb& other) {
        Argb co;
        co.a = blendAlpha(other.a, a);
        co.r = blendColor(other.r, other.a, r, a);
        co.g = blendColor(other.g, other.a, g, a);
        co.b = blendColor(other.b, other.a, b, a);
        return co;
    }

    void operator+=(const Argb& other) {
        *this = *this + other;
    }

    operator Rgb() {
        return {r, g, b};
    }

private:

    template<class T>
    static T blendColor(T colorA, T alphaA, T colorB, T alphaB) {
        auto fColorA = normalize(colorA);
        auto fAlphaA = normalize(alphaA);
        auto fColorB = normalize(colorB);
        auto fAlphaB = normalize(alphaB);
        auto fOut = (fColorA * fAlphaA + fColorB * fAlphaB * (1 - fAlphaA)) / (fAlphaA + fAlphaB * (1 - fAlphaA));
        return fOut * std::numeric_limits<T>::max();
    }

    template<class T>
    static T blendAlpha(T alphaA, T alphaB) {
        auto fAlphaA = normalize(alphaA);
        auto fAlphaB = normalize(alphaB);
        auto fOut = fAlphaA + fAlphaB * (1 - fAlphaA);
        return fOut * std::numeric_limits<T>::max();
    }

public:
    static const Argb kTransparent, kBlack;
};
}

