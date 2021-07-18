#include <rtthread.h>

static void crash() {
    *((int*)0) = 233;
}

MSH_CMD_EXPORT(crash,);

