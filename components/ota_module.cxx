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

}

void OtaModule::start(std::string_view version, std::shared_ptr<IStream> stream, int size, int offset) {
    target.version = string{version};
    target.size = size;
}

const OtaModule::Target& OtaModule::getTarget() const {
    return target;
}

int OtaModule::retryFetch(std::shared_ptr<IStream> stream, rt_uint8_t* data, int len, int rollback) {
    do {
        try {
            return stream->readData(data, len);
        } catch(exception& e) {
            stream->seek(rollback);
        }
    } while(true);
}

void OtaModule::setProgress(int value) {
    outer->updateProgress(shared_from_this(), value);
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
