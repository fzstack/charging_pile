#include "app.hxx"
#include "app_preset.hxx"
#include "thing.hxx"

using namespace std;

void runApp() {
#ifdef RUN_APP
    Preset::App::get()->run();
#endif
}

namespace __cxxabiv1 {
    std::terminate_handler __terminate_handler = []{ while(true); };
}

