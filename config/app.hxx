#ifndef CONFIG_APP_HXX_
#define CONFIG_APP_HXX_
#include <string>
#include <utilities/nat_port.hxx>

namespace Config {
struct Cloud {
    std::string deviceName = "prototype";
    std::string productKey = "a1SvHKhmFbS";
    std::string deviceSecret = "ecc2b3bcfa8311585bc8818acf7239ce";
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

struct Backup {
    int leftSeconds = 0;
    int timerId = 0;
    float consumption = 0;
};

struct CurrentData {
    NatPort port;
    int timerId;
    int leftMinutes;
    State::Value state;
    int current;
    int voltage;
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

struct DevConfig {
    int currentLimit;
    int uploadThr;
    int fuzedThr;
    int noloadCurrThr;
    int doneCurrThr;
};

std::string getStateStr(State::Value state);


#endif /* CONFIG_APP_HXX_ */
