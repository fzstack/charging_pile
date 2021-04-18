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
#include <components/ota_lower_module.hxx>
using namespace std;

Ota::Ota(shared_ptr<SharedThread> thread): thread(thread) {
    onDone += [this](auto module){
        running = false;
    };

    onError += [this](auto module, auto e, auto desc){
        running = false;
    };
}

void Ota::start(std::string_view version, std::string_view module, std::shared_ptr<IStream> stream, int size) {
    auto mod = getModule(module);
    if(mod == nullptr) return;
    mod->getVersion([this, mod, version = string{version}, stream, size](auto v) {
        auto currVer = get_if<std::string>(&v);
        if(currVer == nullptr || *currVer == version) return;
        running = true;
        mod->start(version, stream, size);
    });
}

bool Ota::isRunning() {
    return running;
}

shared_ptr<OtaModule> Ota::getModule(string_view module) {
    if(module == "upper") {
        return make_shared<OtaUpperModule>(this);
    } else if(module == "lower") {
        return make_shared<OtaLowerModule>(this);
    }
    rt_kprintf("module not found\n");
    return nullptr;
}


const char* Ota::kPartiName = "download";


