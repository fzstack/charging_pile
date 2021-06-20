#include "app.hxx"
#include <map>
#include <utilities/err.hxx>
#include <ui/ws_screen.hxx>
#include <ui/rgb_light_widget.hxx>
#include <ui/loading_widget.hxx>
#include <ui/state_noti_widget.hxx>
#include <ui/progress_widget.hxx>

using namespace Config;
using namespace std;

#ifdef UPPER_END
const char* moduleVersion = "0.2.b9";
#endif
#ifdef LOWER_END
const char* moduleVersion = "0.2.14";
#endif

namespace Preset {
const int
    WsScreen::kWidth = 5,
    WsScreen::kHeight = 2;

const int
    LoadingWidget::zIndex = 2,
    ProgressWidget::zIndex = 1,
    StateNotiWidget::zIndex = 0;

}

namespace Config {
const char
    *Cloud::productKey = "g8smgnhaYeu",
    *Cloud::productSecret = "rr7HU7OTPL5mNdfx";
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
