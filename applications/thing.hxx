/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-01-01     imgcr       the first version
 */
#ifndef APPLICATIONS_THING_HXX_
#define APPLICATIONS_THING_HXX_

#include <array>
#include <components/charger.hxx>
#include <config/bsp.hxx>
#include <memory>

class Thing {
public:
    Thing(std::array<std::shared_ptr<Charger>, Config::Bsp::kPortNum> chargers);
};



#endif /* APPLICATIONS_THING_HXX_ */
