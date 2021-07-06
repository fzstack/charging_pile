#include <components/voice_notifier.hxx>
#include <utilities/cmd.hxx>

#define LOG_TAG "test.voice"
#define LOG_LVL LOG_LVL_DBG
#include <ulog.h>

void test_voice_notifier(int argc, char** argv) {
    ASSERT_MIN_NARGS(2);

    auto data = (rt_uint8_t)atoi(argv[1]);
    ASSERT_ARG(data, 0 <= data && data <= 15);

    auto notifier = Preset::VoiceNotifier::get();
    notifier->wtn6->write(data);
}

MSH_CMD_EXPORT(test_voice_notifier, test_voice_notifier);
