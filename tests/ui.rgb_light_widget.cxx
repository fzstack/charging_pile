#include <ui/rgb_light_widget.hxx>
#include <utilities/cmd.hxx>
#include <rtthread.h>

#define LOG_TAG "test.rgb_wgt"
#define LOG_LVL LOG_LVL_DBG
#include <ulog.h>

void test_rgb_light_widget(int argc, char** argv) {
    ASSERT_MIN_NARGS(4);
    auto r = atoi(argv[1]) != 0;
    auto g = atoi(argv[2]) != 0;
    auto b = atoi(argv[3]) != 0;

    auto widget = Preset::RgbLightWidget<0>::get();
    widget->r = r;
    widget->g = g;
    widget->b = b;
}

MSH_CMD_EXPORT(test_rgb_light_widget, );
