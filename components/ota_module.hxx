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
#include <utilities/nested.hxx>
#include <applications/ota.hxx>
#include <applications/ota_error.hxx>
#include <string_view>
#include <string>
#include <memory>
#include <utilities/istream.hxx>
#include <components/timer.hxx>
#include <functional>
#include <variant>

class Ota;

class OtaModule: public Nested<Ota>, public std::enable_shared_from_this<OtaModule> {
    friend class Ota;
public:
    OtaModule(outer_t* outer);
    virtual void start(std::string_view version, std::shared_ptr<IStream> stream, int size, int offset);
    virtual std::string getName() = 0;
    virtual void getVersion(std::function<void(std::variant<std::string, std::exception_ptr>)> cb) = 0;

    struct Target {
        int size;
        std::string version;
    };

    const Target& getTarget() const;

protected:
    void setProgress(int value);
    std::shared_ptr<SharedThread> getThread();
    void emitError(OtaError e, std::string_view desc);
    void emitDone();

    int retryFetch(std::shared_ptr<IStream> stream, rt_uint8_t* data, int len, int rollback);
private:
    Target target;
    int lastProgress = 0; /* used by ota */
};


