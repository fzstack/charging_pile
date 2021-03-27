#ifndef COMPONENTS_VIRTUAL_LOAD_DETECTOR_HXX_
#define COMPONENTS_VIRTUAL_LOAD_DETECTOR_HXX_

#include <devices/load_detector.hxx>
#include <components/multimeter.hxx>
#include <devices/relay.hxx>
#include <utilities/observable.hxx>
#include <Mutex.h>
#include "timer.hxx"
#include <memory>

class VirtualLoadDetector {
public:
    VirtualLoadDetector(std::shared_ptr<LoadDetector> physical, std::shared_ptr<Relay> relay, std::shared_ptr<Multimeter::Channel> multimeterChannel);
    void init();

private:
    void update();
    void createTimer();

    Observable<bool> inited = {false};
    std::shared_ptr<LoadDetector> physical;
    std::shared_ptr<Relay> relay;
    std::shared_ptr<Multimeter::Channel> multimeterChannel;
    Observable<std::optional<bool>> state;
public:
    Observer<std::optional<bool>> oState;
private:
    std::shared_ptr<Timer> timer = nullptr;
    static rtthread::Mutex mutex;

private:
    static constexpr int kDelayFactor = 2;
    static constexpr int kRelayOffStateUpdateDelayMs = LoadDetector::kDetectWndMs * kDelayFactor;

    static const char* kMutex;
    static const char* kTimer;
};

#include <utilities/singleton.hxx>
#include <config/bsp.hxx>
#include <tuple>
namespace Preset {
template<int R>
class VirtualLoadDetector: public Singleton<VirtualLoadDetector<R>>, public Config::Bsp::assert_t<::VirtualLoadDetector, R> {
    friend class Singleton<VirtualLoadDetector<R>>;
    VirtualLoadDetector(): ::VirtualLoadDetector(LoadDetector<R>::get(), Relay<R>::get(), MultiMeterChannel<R>::get()) {
        ::VirtualLoadDetector::init();
    }
};
}


#endif /* COMPONENTS_VIRTUAL_LOAD_DETECTOR_HXX_ */
