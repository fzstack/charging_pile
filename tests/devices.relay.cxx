/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-12-14     imgcr       the first version
 */
#ifdef TEST_RELAY
#include <devices/relay.hxx>
#include <utilities/cmd.hxx>
#include <map>
#include <string>

#define LOG_TAG "test.relay"
#define LOG_LVL LOG_LVL_DBG
#include <ulog.h>

void test_relay(int argc, char** argv) {
    ASSERT_MIN_NARGS(3);

    auto relayA = Preset::Relay<0>::get();
    auto relayB = Preset::Relay<1>::get();

    auto sel = std::unordered_map<std::string, std::shared_ptr<Relay>> {
        {"A", relayA},
        {"B", relayB},
    };

    auto found = sel.find(argv[1]);
    ASSERT_ARG(relay_name, found != sel.end());

    auto value = atoi(argv[2]) != 0;
    **found->second = value;
}

MSH_CMD_EXPORT(test_relay, );
#endif
