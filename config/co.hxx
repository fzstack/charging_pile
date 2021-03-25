#ifndef CONFIG_CO_HXX_
#define CONFIG_CO_HXX_

#include "bsp.hxx"
#include "app.hxx"
#include <components/packet.hxx>
#include <components/rpc.hxx>
#include <applications/cloud.hxx>
#include <string>
#include <optional>
#include <array>
#include <memory>
#include <utilities/inner_port.hxx>

#if (defined(LOWER_END) && defined(UPPER_END)) || (!defined(LOWER_END) && !defined(UPPER_END))
#error "Only one of LOWER_END or UPPER_END can be defined"
#endif

namespace Packets {
template<int N>
struct State {
    ::State::Value value;
};

namespace Props {
struct CurrentData {
    std::array<::CurrentData, Config::Bsp::kPortNum> value;
};
}

namespace Events {
struct PortAccess {
    InnerPort port;
};

struct PortUnplug {
    InnerPort port;
};

struct IcNumber {
    InnerPort port;
    std::string icNumber;
};

struct CurrentLimit {
    InnerPort port;
};

struct CurrentData {
    ::CurrentData value;
};
}
}

namespace Rpcs {
namespace Services {
struct Query {

};

struct Control {
    InnerPort port;
    int timerId;
    int minutes;
};

struct Stop {
    InnerPort port;
    int timerId;
};

struct Config {
    int currentLimit;
    int uploadThr;
    int fuzedThr;
    int noloadCurrThr;
};
}

namespace PersistentStorage {
template<class T>
struct Make {

};
}

}

template<class T>
struct RpcTrait<Rpcs::PersistentStorage::Make<T>> {
    using result_t = std::shared_ptr<T>;
};

#endif /* CONFIG_CO_HXX_ */
