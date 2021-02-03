/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-02-03     imgcr       the first version
 */
#pragma once

#include <utilities/singleton.hxx>
#include "app.hxx"
#include "upper_app.hxx"
#include "lower_app.hxx"

namespace Preset {

struct WhichApp {
#ifdef UPPER_END
    using type = UpperApp;
#else
    using type = LowerApp;
#endif
};

class App: public Singleton<App>, public WhichApp::type {
    friend class Singleton<App>;
    App(): WhichApp::type(){}
};
}
