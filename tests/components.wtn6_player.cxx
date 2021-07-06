#include <components/wtn6_player.hxx>
#include <rtthread.h>
#include <utilities/cmd.hxx>
#include <string>

using namespace std;

void test_wtn6_player(int argc, char** argv) {
    auto player = Preset::Wtn6Player::get();
    Cmd{argc, argv}([&](Cmd& cmd){
        cmd.select<string>({
            {"amount", [&](){
                auto amount = cmd.get<int>();
                player->play(std::move(Voice{}.amount(amount)), Player::Level::Important);
            }},
            {"unplug", [&](){
                auto port = NatPort{cmd.get<rt_uint8_t>()};
                player->play(std::move(Voice{}.port(port).fragm(VoiceFragment::Unpluged)), Player::Level::Ignorable);
            }},
            {"plug", [&](){
                auto port = NatPort{cmd.get<rt_uint8_t>()};
                player->play(std::move(Voice{}.port(port).fragm(VoiceFragment::Pluged)), Player::Level::Ignorable);
            }},
        });
    });
}

MSH_CMD_EXPORT(test_wtn6_player,);
