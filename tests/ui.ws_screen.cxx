#include <ui/ws_screen.hxx>
#include <utilities/cmd.hxx>

#define LOG_TAG "test.ws_scrn"
#define LOG_LVL LOG_LVL_DBG
#include <ulog.h>

using namespace std;

void test_ws_screen(int argc, char** argv) {
    //x y r g b
    ASSERT_MIN_NARGS(6);
    auto x = atoi(argv[1]);
    auto y = atoi(argv[2]);
    auto r = (rt_uint8_t)atoi(argv[3]);
    auto g = (rt_uint8_t)atoi(argv[4]);
    auto b = (rt_uint8_t)atoi(argv[5]);
    auto screen = Preset::WsScreen::get();
    screen->setPixel({r, g, b}, x, y);
    screen->flush();
}

int init_test_ws_screen() {
    auto screen = Preset::WsScreen::get();
    return RT_EOK;
}

MSH_CMD_EXPORT(test_ws_screen, );
INIT_APP_EXPORT(init_test_ws_screen);
