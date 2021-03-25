#ifndef COMPONENTS_RGB_LIGHT_HXX_
#define COMPONENTS_RGB_LIGHT_HXX_

#include <utilities/observable.hxx>
#include <optional>

class RgbLight {
public:
    RgbLight() {}

    Observable<std::optional<bool>> r = {}, g = {}, b = {};
};



#endif /* COMPONENTS_RGB_LIGHT_HXX_ */
