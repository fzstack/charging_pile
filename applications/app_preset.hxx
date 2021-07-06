#pragma once

#include <utilities/singleton.hxx>
#include "app.hxx"
#include "upper_app.hxx"
#include "lower_app.hxx"
#include <rtthread.h>

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
