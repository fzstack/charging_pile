#include "packet.hxx"
#include <string>
#include <stdexcept>
#include <Lock.h>

using namespace std;
using namespace string_literals;

Packet::Packet(std::shared_ptr<QueuedUart> uart, std::shared_ptr<Thread> pktThread): uart(uart), pktThread(pktThread) {
    pktThread->onRun += [this](){
        while(true) {
#ifdef __cpp_exceptions
            try {
#endif
                handleFrame();
#ifdef __cpp_exceptions
            } catch(const exception& e) {
                rt_kprintf("\033[33m[%s] %s\n\033[0m", rt_thread_self()->name, e.what());
            }
#endif
        }
    };
    pktThread->start();
}

void Packet::handleFrame() {
    auto absorber = make_shared<Absorber>(this);
    if(absorber->isInvalid()) {
        rt_kprintf("type info not found\n");
        return;
    }
    auto hash = absorber->getHash();
#ifdef LOG_PKG_ABSORB
    //rt_kprintf("h:%08x\n", hash);
#endif
    auto found = typeInfos.find(hash);
    if(found == typeInfos.end()) {
#ifdef __cpp_exceptions
        throw type_info_not_found_error{"type info not found"};
#else
        rt_kprintf("type info not found");
        return;
#endif
    }
    auto& info = found->second;
    auto p = info.parser->parse(absorber);
    if(!absorber->check()) {
#ifdef __cpp_exceptions
        throw invalid_frame_error{"invalid frame"};
#else
        rt_kprintf("invalid frame");
        return;
#endif
    }
#ifdef LOG_PKG_ABSORB
    rt_kprintf("\n");
#endif
#ifdef LOG_PKG_CB
    rt_kprintf("try invoke pkg cb...\n");
#endif
    //cbThread->exec([cb = info.callback, p]{
#ifdef LOG_PKG_CB
        rt_kprintf("invoking pkg cb @%s...\n", rt_thread_self()->name);
#endif
        info.callback->invoke(p);
#ifdef LOG_PKG_CB
        rt_kprintf("pkg cb invoked @%s\n", rt_thread_self()->name);
#endif
    //});
}

Packet::Emitter::Emitter(outer_t* outer, size_t hashCode): nested_t(outer) {
    outer->mutex.lock();
#ifdef LOG_PKG_EMIT
    rt_kprintf("emit: ");
#endif
    writeByte(ControlChar::Head);
    write(hashCode);
}

Packet::Emitter::~Emitter() {
    auto crcActual = crc.get();
    write(crcActual);
#ifdef LOG_PKG_EMIT
    rt_kprintf("\n");
#endif
    outer->mutex.unlock();
}

void Packet::Emitter::writeData(rt_uint8_t* data, int len) {
    for(auto i = 0; i < len; i++) {
        writeByte(data[i]);
    }
}

void Packet::Emitter::writeByte(std::variant<rt_uint8_t, ControlChar> b) {
    if(auto cc = get_if<ControlChar>(&b)) {
        //不需要转义
        writeAtom(rt_uint8_t(*cc));
    } else if(auto dat = get_if<rt_uint8_t>(&b)) {
        if(*dat == rt_uint8_t(ControlChar::Head) || *dat == rt_uint8_t(ControlChar::Escape))
            writeAtom(rt_uint8_t(ControlChar::Escape));
        writeAtom(*dat);
    }
}

void Packet::Emitter::writeAtom(rt_uint8_t b) {
    outer->uart->send(&b, sizeof(b));
#ifdef LOG_PKG_EMIT
    rt_kprintf("%02x ", b);
#endif
    crc.update(b);
}

Packet::Absorber::Absorber(outer_t* outer): nested_t(outer) {
    auto b = readByte();
    auto head = get_if<ControlChar>(&b);
    if(head == nullptr || *head != ControlChar::Head) {
#ifdef __cpp_exceptions
        throw invalid_frame_error{""};
#endif
        invalidate();
    }
        
    if(isInvalid()) {
        return;
    }
    hash = read<size_t>();
}

std::size_t Packet::Absorber::getHash() {
    return hash;
}

int Packet::Absorber::readData(rt_uint8_t* data, int len) {
    if(isInvalid())
        return 0;
    for(auto i = 0; i < len; i++) {
        auto d = readByte();
        if(isInvalid()) //失败就返回
            return 0;
        data[i] = get<rt_uint8_t>(d);
    }
    return len;
}

std::variant<rt_uint8_t, Packet::ControlChar> Packet::Absorber::readByte() {
    if(isInvalid())
        return ControlChar::Invalid;

    auto b = readAtom();
    if(b == rt_uint8_t(ControlChar::Head))
        return ControlChar::Head;

    if(b == rt_uint8_t(ControlChar::Escape)) {
        b = readAtom();
        if(b != rt_uint8_t(ControlChar::Head) && b != rt_uint8_t(ControlChar::Escape)) {
#ifdef __cpp_exceptions
            throw invalid_escape_error{""};
#endif
            invalidate();
        }
    }
    return b;
};

rt_uint8_t Packet::Absorber::readAtom() {
    auto b = rt_uint8_t{};
    outer->uart->recv(&b, sizeof(b), RT_WAITING_FOREVER);
    if(first) {
        first = false;
#ifdef LOG_PKG_ABSORB
        rt_kprintf("absorb: ");
#endif
    }
#ifdef LOG_PKG_ABSORB
    rt_kprintf("%02x ", b);
#endif
    crc.update(b);
    return b;
}

bool Packet::Absorber::check() {
    auto crcExpect = crc.get();
    auto crcActual = read<rt_uint16_t>();
    return crcExpect == crcActual;
}

void Packet::Absorber::invalidate() {
    invalid = true;
}

bool Packet::Absorber::isInvalid() {
    return invalid;
}

bool Packet::checkConnected() {
    //FIXME: bug来自这里
    return Preset::Handshake::get()->oConnected;
}

namespace Preset {
Packet::Packet(): ::Packet(std::make_shared<QueuedUart>(kUart, getConf()), std::make_shared<::Thread>(kThreadStack, kThreadPrio, kThreadTick, kThread)) {

}
}