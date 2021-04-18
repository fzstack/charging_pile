/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-04-14     imgcr       the first version
 */
#pragma once
#include "ota_module.hxx"
#include <components/rpc.hxx>
#include <components/watch_dog.hxx>
#include <vector>

class OtaLowerModule: public OtaModule {
    using OtaModule::OtaModule;
public:
    virtual void start(std::string_view version, std::shared_ptr<IStream> stream, int size) override;
    virtual std::string getName() override;
    virtual void getVersion(std::function<void(std::variant<std::string, std::exception_ptr>)> cb) override;
private:
    std::shared_ptr<Rpc> rpc = Preset::Rpc::get();
    std::shared_ptr<WatchDog> dog = Preset::WatchDog::get();
    int recvedLen = 0, currPos = 0, testRemain = 147072;
    std::vector<rt_uint8_t> buf = std::vector<rt_uint8_t>(kBufSize);
    std::function<void(std::variant<Void, std::exception_ptr>)> cb;
    static constexpr int kBufSize = 128;
};


