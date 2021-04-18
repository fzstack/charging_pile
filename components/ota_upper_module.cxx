/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-04-14     imgcr       the first version
 */
#include "ota_upper_module.hxx"
extern "C" {
#include <fal.h>
}
#include <config/app.hxx>

using namespace std;

void OtaUpperModule::start(std::string_view version, std::shared_ptr<IStream> stream, int size) {
    getThread()->exec([this, self = shared_from_this(), stream, size] () mutable {
        try {
            rt_kprintf("UPPER OTA RUNNING...\n");
            auto parti = fal_partition_find(kPartiName);
            if(parti == nullptr) {
                emitError(OtaError::UpdateFailed, "分区不存在");
                return;
            }
            rt_kprintf("erasing parti...\n");
            if(fal_partition_erase(parti, 0, size) < 0) {
                emitError(OtaError::UpdateFailed, "分区擦除失败");
                return;
            }

            vector<rt_uint8_t> buf(kBufSize);
            int recvedLen, currPos = 0;
            rt_kprintf("downloading...\n");
            while((recvedLen = stream->readData(&buf[0], kBufSize)) != 0) {
                fal_partition_write(parti, currPos, &buf[0], recvedLen);
                currPos += recvedLen;
                setProgress(100 * currPos / size);
            }
            stream = nullptr;
            rt_kprintf("stream disposed\n");
            if(currPos == size) {
                emitDone();
            } else {
                emitError(OtaError::DownloadFailed, "下载失败");
            }
        } catch(const exception& e) {
            emitError(OtaError::DownloadFailed, "下载失败");
        }
    });
}

std::string OtaUpperModule::getName() {
    return "upper";
}

void OtaUpperModule::getVersion(std::function<void(std::variant<std::string, std::exception_ptr>)> cb) {
    cb(::moduleVersion);
}

const char* OtaUpperModule::kPartiName = "download";
