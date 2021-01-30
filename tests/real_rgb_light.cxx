/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-12-13     imgcr       the first version
 */

#include <rtthread.h>
#include "real_rgb_light.hxx"
#include <utilities/cmd.hxx>
#include <map>
#include <string>
#include <optional>


#define LOG_TAG "test.rgb_light"
#define LOG_LVL LOG_LVL_DBG
#include <ulog.h>

using namespace std;

#ifdef TEST_REAL_RGB_LIGHT
static void test_real_rgb_light(int argc, char** argv) {
    ASSERT_MIN_NARGS(3);

    auto rgbLightA = Preset::RealRgbLight<0>::get();
    auto rgbLightB = Preset::RealRgbLight<1>::get();

    auto m = map<string, Observable<optional<bool>>&>{
        {"A-R", rgbLightA->r}, {"A-G", rgbLightA->g}, {"A-B", rgbLightA->b},
        {"B-R", rgbLightB->r}, {"B-G", rgbLightB->g}, {"B-B", rgbLightB->b},
    };

    auto light = m.find(argv[1]);
    ASSERT_ARG(light_pos, light != m.end());

    auto state = atoi(argv[2]);
    light->second = state != 0;
}


MSH_CMD_EXPORT(test_real_rgb_light, );
#endif
