#ifndef CONFIG_APP_HXX_
#define CONFIG_APP_HXX_
#include <string>
#include <utilities/nat_port.hxx>

namespace Config {
class App {
public:
    static const char
        *cloudDeviceName,
        *cloudProductKey,
        *cloudDeviceSecret;
};

}

struct State {
    enum Value {
        LoadNotInsert = 1,
        LoadInserted,
        Charging,
        LoadWaitRemove,
        Error,
    };
};

struct CurrentData {
    NatPort port;
    int timerId;
    int leftMinutes;
    State::Value state;
    float current;
    float voltage;
    float consumption;
    struct Fuse {
        enum Value {
            Normal,
            Fused,
            Unknown,
        };
    };
    Fuse::Value fuse;
};

struct Heartbeat {
    int signal;
    std::string imeiSuff;
    int temperature;
    int humidity;
    int smoke;
    int timestamp;

};

std::string getStateStr(State::Value state);


#endif /* CONFIG_APP_HXX_ */
