#include "local_debugger.hxx"
#include <config/co.hxx>

LocalDebugger::LocalDebugger(std::shared_ptr<Rebooter> rebooter, std::shared_ptr<Packet> packet): rebooter(rebooter), packet(packet) {

}

void LocalDebugger::debug(DebugType type) {
    switch(type) {
    case DebugType::Reboot:
        rebooter->rebootAll();
        break;
    case DebugType::ClearFlash:
        //TODO: 考虑一下清除哪些配置?
        break;
    case DebugType::CrashUpper:
        *((int*)nullptr) = 0;
        break;
    case DebugType::CrashLower:
        packet->emit<Packets::Debugger>({
            type: type,
        });
        break;
    }
}