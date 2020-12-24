/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-08-25     imgcr       the first version
 */
#ifndef DEVICES_HLW8112_HXX_
#define DEVICES_HLW8112_HXX_

#include "hlw8112_regs.hxx"
#include "queued_uart.hxx"
#include <memory>
#include <utilities/observable.hxx>
#include <optional>
#include <utilities/nested.hxx>
#include <cstring>

class Hlw8112 {
public:
    Hlw8112(const char* uartDeviceName);
    void init();
private:
    template<class T>
    class Session: public Nested<Hlw8112> {
    public:
        using reg_type = typename T::type;
        Session(Hlw8112* outer): Nested<Hlw8112>(outer), reg(read()), regInit(reg) { }

        ~Session() { commit(); }

        void commit() {
            if(T::readonly)
                return;
            if(compare(reg, regInit))
                return;
            outer->writeReg(T::addr, &reg, getSize());
        }

        reg_type* operator->() { return &reg; }
        reg_type& operator*() { return reg; }

    private:
        reg_type read() { //TODO: timeout
            reg_type reg;
            outer->readReg(T::addr, &reg, getSize());
            return reg;
        }

        bool compare(reg_type a, reg_type b) {
            return !memcmp(&a, &b, getSize());
        }

        constexpr int getSize() {
            return T::size == 0 ? sizeof(reg_type) : T::size;
        }

    private:
        reg_type reg, regInit;
        bool modified;
    };

public:
    template<class T>
    Session<T> makeSess() {
        return Session<T>(this);
    }

private:
    void writeEnable();
    void writeDisable();
    void cmd(int cmd, void* data, int len);
    void specCmd(char cmd);

    rt_err_t readReg(char addr, void* data, int len, rt_int32_t timeout = RT_WAITING_FOREVER);
    void writeReg(int addr, void* data, int len);

public:
    Observable<std::optional<bool>> writable;

private:
    std::shared_ptr<QueuedUart> uart;
};

#endif /* DEVICES_HLW8112_HXX_ */
