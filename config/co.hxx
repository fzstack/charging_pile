/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-01-27     imgcr       the first version
 */
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

#if (defined(LOWER_END) && defined(UPPER_END)) || (!defined(LOWER_END) && !defined(UPPER_END))
#error "Only one of LOWER_END or UPPER_END can be defined"
#endif

namespace Packets {
template<int N>
struct State {
    ::State::Value value;
};

namespace Devices {
struct Wtn6Write {
    rt_uint8_t value;
};

struct Rc522 {
    std::optional<std::string> cardId;
};
}

namespace Props {
struct CurrentData {
    std::array<::CurrentData, Config::Bsp::kPortNum> value;
};
}

namespace Events {
struct PortAccess {
    int port;
};

struct PortUnplug {
    int port;
};

struct IcNumber {
    int port;
    std::string icNumber;
};

struct CurrentLimit {
    int port;
};

struct CurrentData {

};
}
}

namespace Rpcs {
namespace Services {
struct Query {

};

struct Control {
    int port;
    int timerId;
    int minutes;
};

struct Stop {
    int port;
    int timerId;
};

struct Config {
    int currentLimit;
    int uploadThr;
    int fuzedThr;
    int noloadCurrThr;
};

struct GetCurrentData {

};
}

namespace PersistentStorage {
template<class T>
struct Make {

};
}

}

template<>
struct RpcTrait<Rpcs::Services::GetCurrentData> {
    using result_t = std::array<CurrentData, Config::Bsp::kPortNum>;
};

template<class T>
struct RpcTrait<Rpcs::PersistentStorage::Make<T>> {
    using result_t = std::shared_ptr<T>;
};

#endif /* CONFIG_CO_HXX_ */
