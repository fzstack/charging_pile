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

using namespace std;

Ota::Ota(shared_ptr<SharedThread> thread): thread(thread) {
    timer.onRun += [=]{
        if(*progress != innerProgress) {
            progress = innerProgress;
        }
    };

    timer.start();
}

void Ota::start(string_view version, string_view module, shared_ptr<IStream> stream, int size) {
    thread->exec([this, stream, size]{
        try {
            rt_kprintf("OTA RUNNING...\n");
            innerProgress = 0;
            progress = 0;
            running = true;
            auto guard = shared_ptr<void>(nullptr, [this](auto) {
                running = false;
            });
            auto parti = fal_partition_find(kPartiName);
            if(parti == nullptr) {
                onError(Error::UpdateFailed, "分区不存在");
                return;
            }
            rt_kprintf("erasing parti...\n");
            if(fal_partition_erase(parti, 0, size) < 0) {
                onError(Error::UpdateFailed, "分区擦除失败");
                return;
            }

            vector<rt_uint8_t> buf(kBufSize);
            int recvedLen, currPos = 0;

            rt_kprintf("downloading...\n");
            while((recvedLen = stream->readData(&buf[0], kBufSize)) != 0) {
                fal_partition_write(parti, currPos, &buf[0], recvedLen);
                currPos += recvedLen;
                innerProgress = 100 * currPos / size;
            }
            if(currPos == size) {
                onDone();
            } else {
                onError(Error::DownloadFailed, "下载失败");
            }
        } catch(const exception& e) {
            onError(Error::DownloadFailed, "下载失败");
        }

    });
}

bool Ota::isRunning() {
    return running;
}

const char* Ota::kPartiName = "download";


