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

static int ota_app_vtor_reconfig(void)
{
    #define NVIC_VTOR_MASK   0x3FFFFF80
    /* Set the Vector Table base location by user application firmware definition */
    SCB->VTOR = 0x08009000 & NVIC_VTOR_MASK;

    return 0;
}
INIT_BOARD_EXPORT(ota_app_vtor_reconfig);
