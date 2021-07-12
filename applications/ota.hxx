/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-04-13     imgcr       the first version
 */
#pragma once
#include <utilities/shared_thread.hxx>
#include <utilities/istream.hxx>
#include <utilities/observer.hxx>
#include <utilities/signals.hxx>
#include <string>
#include <memory>
#include <components/ota_module.hxx>
#include "ota_error.hxx"

class OtaModule;
//ota 需要在单独的线程
class Ota {
    friend class OtaModule;
public:
    Ota(std::shared_ptr<SharedThread> thread);
    std::shared_ptr<OtaModule> getModule(std::string_view module);
    void start(std::string_view version, std::string_view module, std::shared_ptr<IStream> stream, int size);
    bool isRunning();
private:
    int getLastOffset(std::string_view module, std::string_view version);
    void setLastOffset(std::string_view module, std::string_view version, int offset);
    void updateProgress(std::shared_ptr<OtaModule> module, int pos);
public:
    Signals<void(std::string_view module)> onDone = {};
    Signals<void(std::string_view module, int value)> onProgress = {};
    Signals<void(std::string_view module, OtaError e, std::string_view desc)> onError = {};
private:
    std::shared_ptr<SharedThread> thread;
    bool running = false;
    static const char* kPartiName;
    static constexpr int kBufSize = 100;

};

#include <utilities/singleton.hxx>
namespace Preset {
class Ota: public Singleton<Ota>, public ::Ota {
    friend class Singleton<Ota>;
    Ota(): ::Ota(SharedThread<Priority::Low>::get()) {}
};
}

