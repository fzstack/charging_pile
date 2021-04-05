#include <components/persistent_storage_preset.hxx>
#include "fuse_detecter.hxx"
#include <numeric>
#include <cmath>

using namespace std;
using namespace Things::Decos;

FuseDetecter::FuseDetecter(outer_t* outer): Base(outer) {
    inited.onChanged += [this](auto value) {
        if(!value) return;
        timer.onRun += [this] {
            for(rt_uint8_t i = 0u; i < Config::Bsp::kPortNum; i++) {
                auto& info = getInfo(InnerPort{i});
                auto& spec = specs[i];
                auto charger = info.charger;
                if(*charger->stateStore->oState != State::LoadNotInsert) continue;

                auto angle = *charger->multimeterChannel->angle;
                if(!angle) continue;

                auto& angleHist = spec.angleHist;
                angleHist.push_back(*angle);
                if(angleHist.size() < kSatSize) continue;

                angleHist.pop_front();
                auto sum = accumulate(angleHist.begin(), angleHist.end(), 0);
                auto avg = sum / angleHist.size();
                auto s2 = accumulate(angleHist.begin(), angleHist.end(), 0, [&](auto a, auto b) {
                    return a + pow(b - avg, 2);
                });
                rt_kprintf("port%d s2: %d\n", i, int(s2));
            }
        };
        timer.start();
    };
}

void FuseDetecter::init() {
    inited = true;
}

void FuseDetecter::config(DevConfig conf) {
//    auto params = Preset::PersistentStorage::get()->make<Params>();
//    params->fuzedS2Thr = fuzedThr;
}

