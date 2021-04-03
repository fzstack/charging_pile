#include "lower_app.hxx"
#include <components/persistent_storage_preset.hxx>

LowerApp::LowerApp() {

}

void LowerApp::run() {
    rt_kprintf("handshaking...\n");
    handshake->hello();
    rt_kprintf("hello!\n");
    thing->init();
}

