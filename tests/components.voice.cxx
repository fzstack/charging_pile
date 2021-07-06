#include <utilities/f.hxx>
#include <utilities/cmd.hxx>
#include <components/voice.hxx>
#include <rtthread.h>
#include <unordered_map>

using namespace std;

unordered_map<VoiceFragment, const char*> voiceStr = {
    {VoiceFragment::Dot, "点"},
    {VoiceFragment::Zero, "零"},
    {VoiceFragment::One, "一"},
    {VoiceFragment::Two, "二"},
    {VoiceFragment::Three, "三"},
    {VoiceFragment::Four, "四"},
    {VoiceFragment::Five, "五"},
    {VoiceFragment::Six, "六"},
    {VoiceFragment::Seven, "七"},
    {VoiceFragment::Eight, "八"},
    {VoiceFragment::Nine, "九"},
    {VoiceFragment::Ten, "十"},
    {VoiceFragment::Hundred, "百"},
    {VoiceFragment::Thousand, "千"},
    {VoiceFragment::TenThousand, "万"},
    {VoiceFragment::Yuan, "元"},
};

void test_amount(int argc, char** argv) {
    Cmd{argc, argv}([](Cmd cmd) {
        auto cents = cmd.get<int>();
        auto voice = Voice{}.amount(cents);

        for(auto c: voice.seq) {
            auto found = voiceStr.find(c);
            if(found != voiceStr.end()) {
                rt_kprintf("%s", found->second);
            } else {
                rt_kprintf("?");
            }
        }
        rt_kprintf("\n");

    });
}

MSH_CMD_EXPORT(test_amount, );
