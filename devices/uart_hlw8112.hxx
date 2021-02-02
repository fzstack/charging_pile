/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-02-02     imgcr       the first version
 */
#pragma once
#include "hlw8112.hxx"
#include "queued_uart.hxx"
#include <memory>

class UartHlw8112: public Hlw8112 {
public:
    UartHlw8112(const char* uartName);

protected:
    virtual void cmd(int cmd, void* data, int len) override;
    virtual void readReg(char addr, void* data, int len, rt_int32_t timeout = 5) override;

private:
    std::shared_ptr<QueuedUart> uart;
};

#include <utilities/singleton.hxx>
#include "uart_hlw8112.hxx"
namespace Preset {
class UartHlw8112: public Singleton<UartHlw8112>, public ::UartHlw8112 {
    friend class Singleton<UartHlw8112>;
    UartHlw8112(): ::UartHlw8112(kUart){}
    static const char *kUart;
};
}
