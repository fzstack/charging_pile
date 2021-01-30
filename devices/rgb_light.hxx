/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-01-30     imgcr       the first version
 */
#ifndef COMPONENTS_RGB_LIGHT_HXX_
#define COMPONENTS_RGB_LIGHT_HXX_

#include <utilities/observable.hxx>
#include <optional>

class RgbLight {
public:
    RgbLight();

    Observable<std::optional<bool>> r = {}, g = {}, b = {};
};



#endif /* COMPONENTS_RGB_LIGHT_HXX_ */
