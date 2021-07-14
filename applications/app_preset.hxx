#pragma once

#include <utilities/singleton.hxx>
#include "app.hxx"
#include <rtthread.h>

#ifdef UPPER_END
#include "upper_app.hxx"
#else
#include "lower_app.hxx"
#endif

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
