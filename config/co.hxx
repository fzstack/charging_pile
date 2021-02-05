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

#if (defined(LOWER_END) && defined(UPPER_END)) || (!defined(LOWER_END) && !defined(UPPER_END))
#error "Only one of LOWER_END or UPPER_END can be defined"
#endif

namespace Packets {
struct Init {

};

template<int N>
struct State {
    ::State::Value value;
};

namespace Devices {
struct Wtn6Write {
    rt_uint8_t value;
};
}

namespace Props {
struct CurrentData {
    ::CurrentData value[::Config::Bsp::kPortNum];
};

struct Iccid {
    char value[21];
};

struct Signal {
    int value;
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
    char icNumber[9];
};

struct CurrentLimit {
    int port;
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
}
}

template<>
struct RpcTrait<Rpcs::Services::Control> {
    using result_t = bool;
};

template<>
struct RpcTrait<Rpcs::Services::Stop> {
    using result_t = bool;
};



#endif /* CONFIG_CO_HXX_ */
