#include "lower_ota_app.hxx"
#include <rtthread.h>

void LowerOtaApp::run() {
    rt_kprintf("lower ota app running...");
    handshake->hello();
}

std::shared_ptr<Handshake> LowerOtaApp::getHandshake() {
    return handshake;
}