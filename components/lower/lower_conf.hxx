#pragma once
extern "C" {
#include <fal.h>
}

#include <rtthread.h>
#include "kvdb.hxx"
#include <config/app.hxx>
#include <components/fal.hxx>

class LowerConf {
public:
    LowerConf(std::shared_ptr<Fal> fal);

    template<class T>
    struct Meta {
        rt_uint16_t hash;
        T data;
    };

    DevConfig read();
    void write(DevConfig& conf);

private:
    const fal_partition* part;
public:
    static const char* partName;
};


#include <utilities/singleton.hxx>
namespace Preset {
class LowerConf: public Singleton<LowerConf>, public ::LowerConf {
    friend singleton_t;
    LowerConf(): ::LowerConf(Fal::get()) { }
};
}
