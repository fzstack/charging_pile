/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-01-27     imgcr       the first version
 */

#include "packet.hxx"
#include <stdexcept>
#include <Lock.h>

using namespace std;

Packet::Packet(std::shared_ptr<QueuedUart> uart, std::shared_ptr<Thread> thread): uart(uart), thread(thread) {
    thread->onRun += [this](){
        while(true) {
            try {
                handleFrame();
            } catch(const exception& e) {
                rt_kprintf("\033[33m[%s]{%s}%s\n\033[0m", rt_thread_self()->name, typeid(e).name(), e.what());
            }
        }
    };
    thread->start();
}

void Packet::handleFrame() {
    auto head = (ControlChar*){};
    while(true) {
        recvCrc.reset();
        auto b = readByte();
        head = get_if<ControlChar>(&b);
        if(head != nullptr && *head == ControlChar::Head) break; //改成do while这里会莫名其妙有个warning
    }

    auto hashCode = read<size_t>();
    auto found = typeInfos.find(hashCode);
    if(found == typeInfos.end()) throw type_info_not_found_error{""};
    auto& info = found->second;
    auto buff = shared_ptr<rt_uint8_t[]>(new rt_uint8_t[info.size]);
    readData(buff.get(), info.size);

    auto crcExpect = recvCrc.get();
    auto crcActual = read<rt_uint16_t>();

    if(crcExpect != crcActual) throw invalid_frame_error{""};
    info.callback->invoke(buff);
}

void Packet::emitInternal(std::size_t hashCode, rt_uint8_t* data, int len) {
    auto guard = rtthread::Lock{mutex};
    rt_kprintf("emit: ");
    sendCrc.reset();
    writeByte(ControlChar::Head);
    write(hashCode);
    writeData(data, len);
    auto crcActual = sendCrc.get();
    write(crcActual);
    rt_kprintf("\n");
}

void Packet::readData(rt_uint8_t* data, int len) {
    for(auto i = 0; i < len; i++) {
        data[i] = get<rt_uint8_t>(readByte());
    }
}

void Packet::writeData(rt_uint8_t* data, int len) {
    for(auto i = 0; i < len; i++) {
        writeByte(data[i]);
    }
}

std::variant<rt_uint8_t, Packet::ControlChar> Packet::readByte() {
    auto b = readAtom();
    if(b == rt_uint8_t(ControlChar::Head))
        return ControlChar::Head;

    if(b == rt_uint8_t(ControlChar::Escape)) {
        b = readAtom();
        if(b != rt_uint8_t(ControlChar::Head) && b != rt_uint8_t(ControlChar::Escape))
            throw invalid_escape_error{""};
    }
    return b;
};

void Packet::writeByte(std::variant<rt_uint8_t, ControlChar> b) {
    if(auto cc = get_if<ControlChar>(&b)) {
        //不需要转义
        writeAtom(rt_uint8_t(*cc));
    } else if(auto dat = get_if<rt_uint8_t>(&b)) {
        if(*dat == rt_uint8_t(ControlChar::Head) || *dat == rt_uint8_t(ControlChar::Escape))
            writeAtom(rt_uint8_t(ControlChar::Escape));
        writeAtom(*dat);
    }
}



rt_uint8_t Packet::readAtom() {
    auto b = rt_uint8_t{};
    uart->recv(&b, sizeof(b), RT_WAITING_FOREVER);
    recvCrc.update(b);
    return b;
}

void Packet::writeAtom(rt_uint8_t b) {
    uart->send(&b, sizeof(b));
    rt_kprintf("%02x ", b);
    sendCrc.update(b);
}


