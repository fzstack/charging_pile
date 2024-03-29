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
#include "app.hxx"
#include <config/app.hxx>
#include <rtconfig.h>
#if (defined(LOWER_END) && defined(UPPER_END)) || (!defined(LOWER_END) && !defined(UPPER_END))
#error "Only one of LOWER_END or UPPER_END can be defined"
#endif

#include "co_handshake.hxx"

namespace Packets {

struct Reboot {

};

namespace BackupMan {
struct Write {
    InnerPort port;
    Backup backup;
};
}

struct State {
    InnerPort port;
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

struct Debugger {
    DebugType type;
};

}

namespace Rpcs {

namespace BackupMan {
struct Read {
    InnerPort port;
};
}

struct State {
    InnerPort port;
    ::State::Value value;
};


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
    DevConfig conf;
};

struct ReadConfig {

};
}

namespace PersistentStorage {
template<class T>
struct Make { };

template<class T>
struct Read { };

template<class T>
struct Write {
    T data;
};
}

namespace Ota {
struct Erase {
    int size;
    int offset;
};

struct Write {
    int addr;
    std::vector<rt_uint8_t> data;
};

struct GetVersion { };
}
}

template<class T>
struct RpcTrait<Rpcs::PersistentStorage::Make<T>> {
    using result_t = std::shared_ptr<T>;
};

template<class T>
struct RpcTrait<Rpcs::PersistentStorage::Read<T>> {
    using result_t = T;
};

template<>
struct RpcTrait<Rpcs::BackupMan::Read> {
    using result_t = Backup;
};

template<>
struct RpcTrait<Rpcs::Ota::GetVersion> {
    using result_t = std::string;
};

template<>
struct RpcTrait<Rpcs::Services::ReadConfig> {
    using result_t = DevConfig;
};

#endif /* CONFIG_CO_HXX_ */
