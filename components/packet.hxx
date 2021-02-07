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
#include <utilities/nested.hxx>
#include <boost/pfr.hpp>
#include <type_traits>
#include <vector>
#include <utilities/mp.hxx>
#include <utilities/istream.hxx>
#include <utilities/ostream.hxx>
#include <utilities/serializer.hxx>
#include <utilities/deserializer.hxx>

//包头    类型         值          CRC
//0xa5 4字节 结构体的值  xxx
template <class T>
struct PacketTrait {
    using result_t = void;
};

//需要一个packet线程来获得数据
class Packet {
private:

public:
    Packet(std::shared_ptr<QueuedUart> uart, std::shared_ptr<Thread> thread);

private:
    class Callback {
    public:
        virtual void invoke(std::shared_ptr<void> data) = 0;
    };

    template<class T>
    class CallbackImpl: public Callback {
    public:
        CallbackImpl(std::function<void(std::shared_ptr<T>)> cb): cb(cb) { }
        virtual void invoke(std::shared_ptr<void> data) override {
            cb(std::reinterpret_pointer_cast<T>(data));
        }
    private:
        std::function<void(std::shared_ptr<T>)> cb;
    };

    enum class ControlChar: rt_uint8_t {
        Head = 0xa5,
        Escape = 0xff,
    };

    class Emitter: public Nested<Packet>, public OStream {
    public:
        Emitter(outer_t* outer, std::size_t hashCode);
        ~Emitter();

        virtual void writeData(rt_uint8_t* data, int len) override;
        void writeByte(std::variant<rt_uint8_t, ControlChar> b);
        void writeAtom(rt_uint8_t b);

    private:
        Crc16 crc;
    };
    friend class Emitter;

    template<class T> class ParserImpl;

    class Absorber: public Nested<Packet>, public IStream {
        template<class T> friend class ParserImpl;
    public:
        Absorber(outer_t* outer);
        std::size_t getHash();
        bool check();

        virtual void readData(rt_uint8_t* data, int len) override;
        std::variant<rt_uint8_t, ControlChar> readByte();
        rt_uint8_t readAtom();
    private:
        std::size_t hash;
        Crc16 crc;
    };
    friend class Absorber;

    class Parser {
    public:
        virtual std::shared_ptr<void> parse(std::shared_ptr<Absorber> absorber) = 0;
    };

    template<class T>
    class ParserImpl: public Parser {
    public:
        virtual std::shared_ptr<void> parse(std::shared_ptr<Absorber> absorber) override {
            return std::make_shared<T>(Deserializer{absorber}.parse<T>());
        }
    };

    struct TypeInfo {
        std::shared_ptr<Callback> callback = nullptr;
        std::shared_ptr<Parser> parser = nullptr;
    };

public:
    template<class T>
    void on(std::function<void(std::shared_ptr<T>)> cb) {
        typeInfos.insert({typeid(T).hash_code(), TypeInfo {
            callback: std::make_shared<CallbackImpl<T>>(cb),
            parser: std::make_shared<ParserImpl<T>>()
        }});
    }

    template<class T>
    void emit(T&& t) {
        auto emitter = std::make_shared<Emitter>(this, typeid(T).hash_code());
        Serializer{emitter}.build(t);
    }

    template<class T>
    void emit(T& t) {
        emit(std::move(t));
    }

    template<class T, class... A>
    void emit(A&&... a) {
        emit(T(std::forward<A>(a)...));
    }

private:



    void handleFrame();


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
    rtthread::Mutex mutex;
    std::optional<rt_uint8_t> last = {};
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
