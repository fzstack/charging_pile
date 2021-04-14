/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-04-14     imgcr       the first version
 */

#include "ota_module.hxx"
#include <string>

using namespace std;

OtaModule::OtaModule(outer_t* outer): nested_t(outer) {
    timer.onRun += [=]{
        if(lastProgress != progress) {
            lastProgress = progress;
            emitProgress(progress);
        }
    };
    timer.start();
}

void OtaModule::setProgress(int value) {
    progress = value;
}

std::shared_ptr<SharedThread> OtaModule::getThread() {
    return outer->thread;
}

void OtaModule::emitError(OtaError e, std::string_view desc) {
    outer->onError(getName(), e, string{desc});
}

void OtaModule::emitDone() {
    outer->onDone(getName());
}

void OtaModule::emitProgress(int value) {
    outer->onProgress(getName(), value);
}

