#include <stm32f1xx.h>
#include <rtthread.h>
#include <utilities/cmd.hxx>

static int init_reg() {
    __HAL_RCC_PWR_CLK_ENABLE();
    HAL_PWR_EnableBkUpAccess();
    __HAL_RCC_BKP_CLK_ENABLE();
    // __HAL_RCC_RTC_ENABLE();
    
    return RT_EOK;
}

static void reg(int argc, char** argv) {
    Cmd{argc, argv}([](Cmd& cmd) {
        cmd.select({
            {"read", [&]{
                auto value = HAL_RTCEx_BKUPRead(nullptr, RTC_BKP_DR1);
                rt_kprintf("%d\n", value);
            }},
            {"write", [&]{
                auto value = cmd.get<int>();
                HAL_RTCEx_BKUPWrite(nullptr, RTC_BKP_DR1, value);
            }},
        });
    });
}

INIT_APP_EXPORT(init_reg);
MSH_CMD_EXPORT(reg, );
