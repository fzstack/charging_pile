#include <rtthread.h>
#include <components/fal.hxx>

static int init_fal() {
    auto fal = Preset::Fal::get();
    return RT_EOK;
}

INIT_APP_EXPORT(init_fal);