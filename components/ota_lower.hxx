/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-04-15     imgcr       the first version
 */
#pragma once
#include <components/rpc.hxx>
extern "C" {
#include <fal.h>
}
#include <components/fal.hxx>

class OtaLower {
public:
    OtaLower(std::shared_ptr<Fal> fal, std::shared_ptr<Rpc> rpc);
private:
    std::shared_ptr<Fal> fal;
    std::shared_ptr<Rpc> rpc;
    const fal_partition *parti;
    static const char* kPartiName;
};

#include <utilities/singleton.hxx>
namespace Preset {
class OtaLower: public Singleton<OtaLower>, public ::OtaLower {
    friend singleton_t;
    OtaLower(): ::OtaLower(Fal::get(), Rpc::get()) {}
};
}


