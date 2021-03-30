#include "os.hxx"
#include <applications/cloud_timer.hxx>
#include <components/virtual_load_detector.hxx>
#include <devices/load_detector.hxx>
#include <components/multimeter.hxx>
#include <components/state_store.hxx>
#include <components/rgb_state_notifier.hxx>
#include <applications/thing.hxx>
#include <applications/things/decos/current_limiter.hxx>
#include <applications/things/decos/counter.hxx>
#include <applications/things/decos/backuper.hxx>
#include <applications/things/decos/consumption_measurer.hxx>
#include <applications/things/decos/fuse_detecter.hxx>
#include <applications/things/decos/noload_detecter.hxx>
#include <applications/things/decos/data_setter.hxx>
#include <components/persistent_storage.hxx>
#include <components/packet.hxx>
#include <devices/ttp229.hxx>
#include <utilities/shared_thread.hxx>
#include <ui/ws_screen.hxx>
#include <ui/loading_widget.hxx>
#include <utilities/tween.hxx>
#include <components/rpc.hxx>

using namespace Config;
using namespace Things::Decos;

const char
    *Os::kAliIotDeviceThreadName = "iot_dev",
    *LoadDetector::kTimer = "lodet",
    *VirtualLoadDetector::kMutex = "vlodet",
    *VirtualLoadDetector::kTimer = "vlodet",
    *Multimeter::kTimer = "mulmtr",
    *StateStore::kMutex = "ststor",
    *Counter::kTimer = "tapoff",
    *::Thing::kMutex = "thing",
    *CurrentLimiter::kTimer = "kltdec",
    *Backuper::kTimer = "btdec",
    *DataSetter::kTimer = "dsdec",
    *ConsumptionMeasurer::kTimer = "cmtdec",
    *FuseDetecter::kTimer = "fdtdec",
    *NoloadDetecter::kTimer = "nltdec",
    *PersistentStorage::kMutex = "persto",
    *Packet::kMutex = "packet",
    *RgbStateNotifier::kTimer = "rgbntf",
    *RgbStateNotifier::kMutex = "rgbntf",
    *TweenBase::kTimer = "tween",
    *TweenBase::kMutex = "tween",
    *Rpc::kMutex = "rpc";

namespace Preset {
const rt_tick_t CloudTimer::kTime = 60000;
const char *CloudTimer::kName = "cloud";

const char* Packet::kThread = "packet";

const char* Ttp229::kThread = "ttp229";
const int
    Ttp229::kThreadStack = 1024,
    Ttp229::kThreadPrio = 2,
    Ttp229::kThreadTick = 10;

template<> const char* SharedThread<Priority::High>::kThread = "high";
template<> const int SharedThread<Priority::High>::kThreadStack = 2048;
template<> const int SharedThread<Priority::High>::kThreadTick = 10;

template<> const char* SharedThread<Priority::Middle>::kThread = "middle";
template<> const int SharedThread<Priority::Middle>::kThreadStack = 2048;
template<> const int SharedThread<Priority::Middle>::kThreadTick = 10;

const int WsScreen::kDuration = 20;
const char* WsScreen::kTimer = "wscrn";

const int LoadingWidget::kDuration = 20;
const char* LoadingWidget::kTimer = "lodwgt";
}




