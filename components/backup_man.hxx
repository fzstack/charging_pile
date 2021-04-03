/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-04-02     imgcr       the first version
 */
#pragma once

#include <functional>
#include <utilities/inner_port.hxx>
#include <config/app.hxx>
#include <optional>

class BackupMan {
public:
    virtual void read(InnerPort port, std::function<void(std::optional<Backup>)>) = 0;
    virtual void write(InnerPort port, Backup backup) = 0;
};


