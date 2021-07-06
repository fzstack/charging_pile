#include <rtthread.h>

#include <utilities/inner_port.hxx>
#include <utilities/nat_port.hxx>
#include <utilities/cmd.hxx>
#include <cstdlib>
#include <utilities/f.hxx>
#include <utilities/serializer.hxx>
#include <utilities/deserializer.hxx>
#include <utilities/memory_iostream.hxx>

#define LOG_TAG "test.port"
#define LOG_LVL LOG_LVL_DBG
#include <ulog.h>

void test_port_inner_to_nat(int argc, char** argv) {
    ASSERT_MIN_NARGS(2);
    auto portId = atoi(argv[1]);
    auto innerPort = InnerPort(portId);
    auto natPort = NatPort(innerPort);
    F{} << "nat port val: "_r << natPort.get() << endln;
}

void test_port_nat_to_inner(int argc, char** argv) {
    ASSERT_MIN_NARGS(2);
    auto portId = atoi(argv[1]);
    auto natPort = NatPort(portId);
    auto innerPort = InnerPort(natPort);
    F{} << "inner port val: "_r << innerPort.get() << endln;
}

void test_port_nat_ser(int argc, char** argv) {
    ASSERT_MIN_NARGS(2);
    auto portId = atoi(argv[1]);
    auto port = NatPort{(rt_uint8_t)portId};
    auto stream = std::make_shared<MemoryIOStream>();
    Serializer{stream}.build(port);
    auto des = Deserializer{stream}.parse<NatPort>();
    F{} << "after parse, value: "_r << des.get() << endln;
}

void test_port_inner_ser(int argc, char** argv) {
    ASSERT_MIN_NARGS(2);
    auto portId = atoi(argv[1]);
    auto port = InnerPort{(rt_uint8_t)portId};
    auto stream = std::make_shared<MemoryIOStream>();
    Serializer{stream}.build(port);
    auto des = Deserializer{stream}.parse<InnerPort>();
    F{} << "after parse, value: "_r << des.get() << endln;
}


MSH_CMD_EXPORT(test_port_inner_to_nat, )
MSH_CMD_EXPORT(test_port_nat_to_inner, )
MSH_CMD_EXPORT(test_port_nat_ser, )
MSH_CMD_EXPORT(test_port_inner_ser, )
