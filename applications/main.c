#include <rtthread.h>

#define DBG_TAG "main"
#define DBG_LVL DBG_LOG
#include <rtdbg.h>
#include "app.hxx"

int main(void) {
    runApp();
    return RT_EOK;
}
