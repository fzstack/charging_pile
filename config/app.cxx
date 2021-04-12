#include "app.hxx"
#include <map>
#include <utilities/err.hxx>
#include <ui/ws_screen.hxx>
#include <ui/rgb_light_widget.hxx>
#include <ui/loading_widget.hxx>
#include <ui/state_noti_widget.hxx>

using namespace Config;
using namespace std;

const char
    *::Version::lower = "0.2.5",
    *::Version::upper = "0.2.5";

namespace Preset {
const int
    WsScreen::kWidth = 5,
    WsScreen::kHeight = 2;

const int LoadingWidget::zIndex = 1;
const int StateNotiWidget::zIndex = 0;
}

namespace Config {
const char
    *Cloud::productKey = "a1SvHKhmFbS",
    *Cloud::productSecret = "eNVvWlHUTCInHPIq";
}

string getStateStr(State::Value state) {
    switch(state) {
    case State::LoadNotInsert:
        return "load_not_insert";
    case State::LoadInserted:
        return "load_inserted";
    case State::Charging:
        return "charging";
    case State::LoadWaitRemove:
        return "load_wait_remove";
    case State::Error:
        return "error";
    default:
        throw not_implemented{"unknown_state"};
    }
}
