/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-04-13     imgcr       the first version
 */

#include "ota.hxx"
extern "C" {
#include <fal.h>
}
#include <components/ota_upper_module.hxx>

using namespace std;

Ota::Ota(shared_ptr<SharedThread> thread): thread(thread) {

}

void Ota::start(std::string_view version, std::string_view module, std::shared_ptr<IStream> stream, int size) {
    auto mod = getModule(module);
    if(mod != nullptr)
        mod->start(version, stream, size);
}

shared_ptr<OtaModule> Ota::getModule(string_view module) {
    if(module == "upper") {
        return make_shared<OtaUpperModule>(this);
    } else if(module == "lower") {

    }
    rt_kprintf("module not found\n");
    return nullptr;
}


const char* Ota::kPartiName = "download";


