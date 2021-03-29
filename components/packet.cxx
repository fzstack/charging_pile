#include "packet.hxx"
#include <string>
#include <stdexcept>
#include <Lock.h>

using namespace std;
using namespace string_literals;

Packet::Packet(std::shared_ptr<QueuedUart> uart, std::shared_ptr<Thread> pktThread): uart(uart), pktThread(pktThread) {
    pktThread->onRun += [this](){
        while(true) {
            try {
                handleFrame();
            } catch(const exception& e) {
                rt_kprintf("\033[33m[%s]{%s}%s\n\033[0m", rt_thread_self()->name, typeid(e).name(), e.what());
            }
        }
    };
    pktThread->start();
}

void Packet::handleFrame() {
    auto absorber = make_shared<Absorber>(this);
    auto hash = absorber->getHash();
#ifdef LOG_PKG_ABSORB
    //rt_kprintf("h:%08x\n", hash);
#endif
    auto found = typeInfos.find(hash);
    if(found == typeInfos.end()) throw type_info_not_found_error{"type info not found"};
    auto& info = found->second;
    auto p = info.parser->parse(absorber);
    if(!absorber->check()) throw invalid_frame_error{info.name};
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
    if(head == nullptr || *head != ControlChar::Head)
        throw invalid_frame_error{""};
    hash = read<size_t>();
}

std::size_t Packet::Absorber::getHash() {
    return hash;
}

void Packet::Absorber::readData(rt_uint8_t* data, int len) {
    for(auto i = 0; i < len; i++) {
        data[i] = get<rt_uint8_t>(readByte());
    }
}

std::variant<rt_uint8_t, Packet::ControlChar> Packet::Absorber::readByte() {
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

