#ifdef TEST_WTN6
#include <rtthread.h>
#include <devices/wtn6.hxx>
#include <utilities/cmd.hxx>
#include <components/voice_notifier.hxx>

#define LOG_TAG "test.wtn6"
#define LOG_LVL LOG_LVL_DBG
#include <ulog.h>

void test_wtn6(int argc, char** argv) {
    ASSERT_MIN_NARGS(2);

    auto wtn6 = Preset::Wtn6::get();
    auto data = atoi(argv[1]);
    ASSERT_ARG(voice_id, 0 <= data && data <= (rt_uint8_t)Voices::Reserved3);
    wtn6->write(data);
}

MSH_CMD_EXPORT(test_wtn6,);
#endif
