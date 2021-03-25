#include "ali_iot_device_thread.hxx"
#include <config/os.hxx>

using namespace Config;
using namespace std;

AliIotDeviceThread::AliIotDeviceThread(outer_t* outer):
  Thread(Os::kAliIotDeviceThreadStackSize, Os::kAliIotDeviceThreadPrio, 20, Os::kAliIotDeviceThreadName), nested_t(outer) {

}

void AliIotDeviceThread::run(void *p) {
    while(true) {
        try {
            post.poll();
        } catch(const exception& e) {
            rt_kprintf("ali_iot: {%s}%s", typeid(e).name(), e.what());
        }
    }

}
