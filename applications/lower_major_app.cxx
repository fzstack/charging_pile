#include "lower_major_app.hxx"
#include <components/persistent_storage_preset.hxx>

LowerMajorApp::LowerMajorApp() {

}

void LowerMajorApp::run() {
    thing->init();
    rt_kprintf("handshaking...\n");
    handshake->hello();
    rt_kprintf("hello!\n");   
}

std::shared_ptr<Handshake> LowerMajorApp::getHandshake() {
    return handshake;
}