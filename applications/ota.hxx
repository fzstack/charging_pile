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
#include <components/timer.hxx>

//ota 需要在单独的线程
class Ota {
public:
    Ota(std::shared_ptr<SharedThread> thread);
    void start(std::string_view version, std::string_view module, std::shared_ptr<IStream> stream, int size);
    bool isRunning();

    Observable<int> progress = 0; //0...99
    Observer<int> oProgress = {progress};
    Signals<void()> onDone;

    enum class Error {
        UpdateFailed = -1,
        DownloadFailed = -2,
    };
    Signals<void(Error e, std::string desc)> onError;
private:
    int innerProgress = 0;
    bool running = false;
    std::shared_ptr<SharedThread> thread;
    static const char* kPartiName;
    static constexpr int kBufSize = 100;
    Timer timer = {1000, "ota"};
};

#include <utilities/singleton.hxx>
namespace Preset {
class Ota: public Singleton<Ota>, public ::Ota {
    friend class Singleton<Ota>;
    Ota(): ::Ota(SharedThread<Priority::Low>::get()) {}
};
}

