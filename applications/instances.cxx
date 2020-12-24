/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-12-08     imgcr       the first version
 */

#include "instances.hxx"
#include <rtthread.h>
#include <config/bsp.hxx>

using namespace std;

#if defined(RUN_APP) || defined(TEST_RGB_LIGHT) || defined(TEST_RGB_STATE_NOTIFIER)
shared_ptr<RgbLight> Instances::rgbLightA = {};
shared_ptr<RgbLight> Instances::rgbLightB = {};
#endif
#if defined(RUN_APP) || defined(TEST_RC522)
shared_ptr<Rc522> Instances::rc522 = {};
#endif
#if defined(RUN_APP) || defined(TEST_RELAY)
shared_ptr<Relay> Instances::relayA = {};
shared_ptr<Relay> Instances::relayB = {};
#endif
#if defined(RUN_APP) || defined(TEST_WTN6) || defined(TEST_VOICE_NOTIFIER)
shared_ptr<Wtn6> Instances::wtn6 = {};
#endif
#if defined(RUN_APP) || defined(TEST_LOAD_DETECTOR)
shared_ptr<LoadDetector> Instances::loadDetectorA = {};
shared_ptr<LoadDetector> Instances::loadDetectorB = {};
#endif
#if defined(RUN_APP) || defined(TEST_HLW8112) || defined(TEST_MULTIMETER)
shared_ptr<Hlw8112> Instances::hlw8112 = {};
#endif
#if defined(RUN_APP) || defined(TEST_AIR724) || defined(TEST_AIR_HTTP_CLIENT) || defined(TEST_AIR_MQTT_CLIENT) || defined(TEST_ALI_IOT_DEVICE)
shared_ptr<Air724> Instances::air724 = {};
#endif
#if defined(RUN_APP) || defined(TEST_VOICE_NOTIFIER)
shared_ptr<VoiceNotifier> Instances::voiceNotifier = {};
#endif
#if defined(RUN_APP) || defined(TEST_RGB_STATE_NOTIFIER)
shared_ptr<RgbStateNotifier> Instances::rgbStateNotifierA = {};
shared_ptr<RgbStateNotifier> Instances::rgbStateNotifierB = {};
#endif
#if defined(RUN_APP) || defined(TEST_MULTIMETER)
shared_ptr<Multimeter> Instances::multimeter = {};
#endif
#if defined(TEST_ALI_IOT_DEVICE)
shared_ptr<AliIotDevice> Instances::aliIotDevice = {};
#endif


int init_instances() {
#if defined(RUN_APP) || defined(TEST_RGB_LIGHT) || defined(TEST_RGB_STATE_NOTIFIER)
    Instances::rgbLightA = make_shared<RgbLight>(Config::Bsp::kRgbLightARPin, Config::Bsp::kRgbLightAGPin, Config::Bsp::kRgbLightABPin);
    Instances::rgbLightB = make_shared<RgbLight>(Config::Bsp::kRgbLightBRPin, Config::Bsp::kRgbLightBGPin, Config::Bsp::kRgbLightBBPin);
#endif
#if defined(RUN_APP) || defined(TEST_RC522)
    Instances::rc522 = make_shared<Rc522>(Config::Bsp::kRc522SpiBus, Config::Bsp::kRc522SpiDev, Config::Bsp::kRc522GpioX, Config::Bsp::kRc522GpioPin);
#endif
#if defined(RUN_APP) || defined(TEST_RELAY)
    Instances::relayA = make_shared<Relay>(Config::Bsp::kRelayAPin);
    Instances::relayB = make_shared<Relay>(Config::Bsp::kRelayBPin);
#endif
#if defined(RUN_APP) || defined(TEST_WTN6) || defined(TEST_VOICE_NOTIFIER)
    Instances::wtn6 = make_shared<Wtn6>(Config::Bsp::kWtn6DataPin, Config::Bsp::kWtn6BusyPin);
#endif
#if defined(RUN_APP) || defined(TEST_LOAD_DETECTOR)
    Instances::loadDetectorA = make_shared<LoadDetector>(Config::Bsp::kLoadDetectorAPin);
    Instances::loadDetectorB = make_shared<LoadDetector>(Config::Bsp::kLoadDetectorBPin);
#endif
#if defined(RUN_APP) || defined(TEST_HLW8112) || defined(TEST_MULTIMETER)
    Instances::hlw8112 = make_shared<Hlw8112>(Config::Bsp::kHlw8112Uart);
#endif
#if defined(RUN_APP) || defined(TEST_AIR724) || defined(TEST_AIR_HTTP_CLIENT) || defined(TEST_AIR_MQTT_CLIENT) || defined(TEST_ALI_IOT_DEVICE)
    Instances::air724 = make_shared<Air724>(Config::Bsp::kAir202Uart, Config::Bsp::kAir202RstPin);
#endif
#if defined(RUN_APP) || defined(TEST_RGB_STATE_NOTIFIER)
    Instances::rgbStateNotifierA = make_shared<RgbStateNotifier>(Instances::rgbLightA);
    Instances::rgbStateNotifierB = make_shared<RgbStateNotifier>(Instances::rgbLightB);
#endif
#if defined(RUN_APP) || defined(TEST_VOICE_NOTIFIER)
    Instances::voiceNotifier = make_shared<VoiceNotifier>(Instances::wtn6);
#endif
#if defined(RUN_APP) || defined(TEST_MULTIMETER)
    Instances::multimeter = make_shared<Multimeter>(Instances::hlw8112);
#endif
#if defined(TEST_ALI_IOT_DEVICE)
    Instances::aliIotDevice = make_shared<AliIotDevice>(Instances::air724->make<HttpClient>(), Instances::air724->make<MqttClient>());
#endif
    return RT_EOK;
}

INIT_ENV_EXPORT(init_instances);
