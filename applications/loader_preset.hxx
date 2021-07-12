#pragma once
#include <utilities/singleton.hxx>
#include "upper_loader.hxx"
#include "lower_loader.hxx"
#include <rtthread.h>

namespace Preset {

struct WhichLoder {
#ifdef UPPER_END
    using type = UpperLoader;
#else
    using type = LowerLoader;
#endif
};

class Loader: public Singleton<Loader>, public WhichLoder::type {
    friend class Singleton<Loader>;
    Loader(): WhichLoder::type(){}
};
}

