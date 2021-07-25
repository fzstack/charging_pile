#include "remote_debugger.hxx"
#include <config/co.hxx>

RemoteDebugger::RemoteDebugger(std::shared_ptr<Rebooter> rebooter, std::shared_ptr<Packet> packet): rebooter(rebooter), packet(packet) {
    packet->on<Packets::Debugger>([=](auto p) {
        switch(p->type) {
        case DebugType::CrashLower:
            *((int*)nullptr) = 0;
            break;
        default:
            break;
        }
    });
}
