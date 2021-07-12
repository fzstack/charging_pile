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
#include <components/persistent_storage_preset.hxx>
#include <config/app.hxx>
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
    auto offset = getLastOffset(module, version);
    stream->seek(offset);
    if(offset != 0) {
        rt_kprintf("continue @%d\n", offset);
    }
    mod->getVersion([this, mod, version = string{version}, stream, size, offset](auto v) {
        auto currVer = get_if<std::string>(&v);
        if(currVer == nullptr || *currVer == version) return;
        running = true;
        mod->start(version, stream, size, offset);
    });
}

bool Ota::isRunning() {
    return running;
}

int Ota::getLastOffset(std::string_view module, std::string_view version) {
    auto storage = Preset::PersistentStorage::get();
    auto conf = storage->read<Config::OTA>();
    auto found = conf.offsets.find(string{module});
    if(found == conf.offsets.end()) {
        return 0;
    }
    auto& spec = found->second;
    if(spec.version == version) {
        return spec.offset;
    }
    return 0;

}

void Ota::setLastOffset(std::string_view module, std::string_view version, int offset) {
    auto storage = Preset::PersistentStorage::get();
    auto conf = storage->read<Config::OTA>();
    conf.offsets[string{module}] = Config::OTA::ModuleSpec{string{version}, offset};
    storage->write(conf);
}

void Ota::updateProgress(std::shared_ptr<OtaModule> module, int pos) {
    auto& target = module->getTarget();
    //每10K保存一次
    static const int kCheckSize = 10 * 1024;
    if(pos - module->lastProgress >= kCheckSize) {
        auto savedPos = pos - (pos % kCheckSize);
        setLastOffset(module->getName(), target.version, savedPos);
        module->lastProgress = savedPos;
        rt_kprintf("OTA checkpoint @0x%04x\n", savedPos);
    }
    
    onProgress(module->getName(), 100 * pos / target.size);
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


