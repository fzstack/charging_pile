/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-12-08     imgcr       the first version
 */
#ifndef APPLICATIONS_INSTANCES_HXX_

#define APPLICATIONS_INSTANCES_HXX_

#include <memory>
#include <optional>

#include <devices/rgb_light.hxx>
#include <devices/rc522.hxx>
#include <devices/relay.hxx>
#include <devices/wtn6.hxx>
#include <devices/load_detector.hxx>
#include <devices/hlw8112.hxx>
#include <devices/air724.hxx>

#include <components/rgb_state_notifier.hxx>
#include <components/voice_notifier.hxx>
#include <components/multimeter.hxx>
#include <components/air_components.hxx>

#include <applications/ali_iot_device.hxx>

class Instances {
public:
    static std::shared_ptr<RgbLight> rgbLightA, rgbLightB;
    static std::shared_ptr<Rc522> rc522;
    static std::shared_ptr<Relay> relayA, relayB;
    static std::shared_ptr<Wtn6> wtn6;
    static std::shared_ptr<LoadDetector> loadDetectorA, loadDetectorB;
    static std::shared_ptr<Hlw8112> hlw8112;
    static std::shared_ptr<Air724> air724;


    static std::shared_ptr<RgbStateNotifier> rgbStateNotifierA, rgbStateNotifierB;
    static std::shared_ptr<VoiceNotifier> voiceNotifier;
    static std::shared_ptr<Multimeter> multimeter;

    static std::shared_ptr<AliIotDevice> aliIotDevice;

};



#endif /* APPLICATIONS2_INSTANCES_HXX_ */
