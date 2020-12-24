/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-12-14     imgcr       the first version
 */

#include "voice_notifier.hxx"
#include <utilities/cmd.hxx>
#include <applications/instances.hxx>

#define LOG_TAG "test.voice"
#define LOG_LVL LOG_LVL_DBG
#include <ulog.h>

#ifdef TEST_VOICE_NOTIFIER
void test_voice_notifier(int argc, char** argv) {
    ASSERT_MIN_NARGS(2);

    auto data = (rt_uint8_t)atoi(argv[1]);
    ASSERT_ARG(data, 0 <= data && data <= 15);

    auto notifier = Instances::voiceNotifier;
    notifier->wtn6->write(data);
}

MSH_CMD_EXPORT(test_voice_notifier, test_voice_notifier);
#endif
