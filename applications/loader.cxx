#include "loader.hxx"
#include "loader_preset.hxx"
#include <stm32f1xx.h>
#include <rthw.h>

void runLoader() {
    auto loader = Preset::Loader::get();
    static auto app = loader->createApp();
    app->run();
}

Loader::Loader() {
     __HAL_RCC_PWR_CLK_ENABLE();
    HAL_PWR_EnableBkUpAccess();
    __HAL_RCC_BKP_CLK_ENABLE();
    startMethod = HAL_RTCEx_BKUPRead(nullptr, RTC_BKP_DR1);
    HAL_RTCEx_BKUPWrite(nullptr, RTC_BKP_DR1, 0);

}

int Loader::getStartMethod() {
    return startMethod;
}

void Loader::restart(int method) {
    HAL_RTCEx_BKUPWrite(nullptr, RTC_BKP_DR1, method);
    rt_hw_cpu_reset();
}
