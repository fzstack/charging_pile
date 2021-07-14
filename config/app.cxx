#include "app.hxx"
#include <map>
#include <utilities/err.hxx>
#include <rtconfig.h>

using namespace Config;
using namespace std;

#ifdef UPPER_END
const char* moduleVersion = UPPER_VERSION;
#endif
#ifdef LOWER_END
const char* moduleVersion = LOWER_VERSION;
#endif


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
        return "unknown";
        // throw not_implemented{"unknown_state"};
    }
}
