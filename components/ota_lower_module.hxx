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

class OtaLowerModule: public OtaModule {
    using OtaModule::OtaModule;
public:
    virtual void start(std::string_view version, std::shared_ptr<IStream> stream, int size) override;
    virtual std::string getName() override;
private:
    std::shared_ptr<Rpc> rpc = Preset::Rpc::get();
    static constexpr int kBufSize = 100;
};


