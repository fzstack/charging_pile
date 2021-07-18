#ifndef CONFIG_APP_HXX_
#define CONFIG_APP_HXX_
#include <string>
#include <utilities/nat_port.hxx>
#include <map>

extern const char* moduleVersion;

namespace Config {
struct Cloud {
    static const char* productKey;
    static const char* productSecret;
};

struct LastVersion {
    std::string upper = "";
};

template<int N = 3>
struct Version {
    bool updated = false;
};

struct AliIotDevice {
    std::string deviceName = "";
    std::string productKey = "";
    std::string deviceSecret = "";
};


struct OTA {
    struct ModuleSpec {
        std::string version;
        int offset;
    };
    std::map<std::string, ModuleSpec> offsets;
};

namespace Loader {
struct OtaParams {
    std::string module;
    std::string version;
    std::string url;
    int size;
};
}
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

enum class BroadcastType {
    Succeed,
    CardNotActivated,
    BalanceInsufficient,
    DeviceAbnormal,
};

std::string getStateStr(State::Value state);


#endif /* CONFIG_APP_HXX_ */
