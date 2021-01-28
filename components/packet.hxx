/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-01-27     imgcr       the first version
 */
#ifndef COMPONENTS_PACKET_HXX_
#define COMPONENTS_PACKET_HXX_

#include <functional>
#include <memory>
#include <variant>
#include <rtthread.h>
#include <devices/queued_uart.hxx>
#include <utilities/thread.hxx>
#include <map>
#include <utilities/crc16.hxx>
#include <Mutex.h>

//包头    类型         值          CRC
//0xa5 4字节 结构体的值  xxx
template <class T>
struct PacketTrait {
    using result_t = void;
};

//需要一个packet线程来获得数据
class Packet {
public:
    Packet(std::shared_ptr<QueuedUart> uart, std::shared_ptr<Thread> thread);

private:
    class Callback {
    public:
        virtual void invoke(std::shared_ptr<rt_uint8_t[]> data) = 0;
    };

    template<class T>
    class CallbackImpl: public Callback {
    public:
        CallbackImpl(std::function<void(std::shared_ptr<T>)> cb): cb(cb) { }
        virtual void invoke(std::shared_ptr<rt_uint8_t[]> data) override {
            cb(std::reinterpret_pointer_cast<T>(data));
        }
    private:
        std::function<void(std::shared_ptr<T>)> cb;
    };

    class TypeInfo {
    public:
        TypeInfo(std::shared_ptr<Callback> callback, std::size_t size): callback(callback), size(size) { }
        std::shared_ptr<Callback> callback = nullptr;
        std::size_t size = 0;
    };

public:
    template<class T>
    void on(std::function<void(std::shared_ptr<T>)> cb) {
        typeInfos.insert({typeid(T).hash_code(), {std::make_shared<CallbackImpl<T>>(cb), size: sizeof(T)}});
    }

    template<class T>
    void emit(T& t) {
        emitInternal(typeid(T).hash_code(), (rt_uint8_t*)(void*)&t, sizeof(T));
    }

    template<class T>
    void emit(T&& t) {
        emitInternal(typeid(T).hash_code(), (rt_uint8_t*)(void*)&t, sizeof(T));
    }

    template<class T, class... A>
    void emit(A&&... a) {
        auto t = T(std::forward<A>(a)...);
        emitInternal(typeid(T).hash_code(), (rt_uint8_t*)(void*)&t, sizeof(T));
    }

    void emitInternal(std::size_t hashCode, rt_uint8_t* data, int len);

private:

    void handleFrame();

    template<class T>
    T read() {
        T t;
        readData((rt_uint8_t*)&t, sizeof(T));
        return t;
    }

    template<class T>
    void write(T&& t) {
        writeData((rt_uint8_t*)(void*)&t, sizeof(T));
    }

    enum class ControlChar: rt_uint8_t {
        Head = 0xa5,
        Escape = 0xff,
    };

    void readData(rt_uint8_t* data, int len);
    void writeData(rt_uint8_t* data, int len);
    std::variant<rt_uint8_t, ControlChar> readByte();
    void writeByte(std::variant<rt_uint8_t, ControlChar> b);
    rt_uint8_t readAtom();
    void writeAtom(rt_uint8_t b);


private:
    class invalid_escape_error: public std::runtime_error {
    public:
        using std::runtime_error::runtime_error;
    };

    class invalid_frame_error: public std::runtime_error {
    public:
        using std::runtime_error::runtime_error;
    };

    class type_info_not_found_error: public std::runtime_error {
    public:
        using std::runtime_error::runtime_error;
    };

private:
    std::shared_ptr<QueuedUart> uart;
    std::shared_ptr<Thread> thread;
    std::map<std::size_t, TypeInfo> typeInfos = {};
    Crc16 recvCrc, sendCrc;
    rtthread::Mutex mutex;
    static const char* kMutex;
};

#include <utilities/singleton.hxx>
namespace Preset {
class Packet: public Singleton<Packet>, public ::Packet {
    friend singleton_t;
    Packet(): ::Packet(std::make_shared<QueuedUart>(kUart), std::make_shared<Thread>(kThreadStack, kThreadPrio, kThreadTick, kThread)) {

    }

    static const char *kUart, *kThread;
    static const int kThreadStack, kThreadPrio, kThreadTick;

};
}


#endif /* COMPONENTS_PACKET_HXX_ */
