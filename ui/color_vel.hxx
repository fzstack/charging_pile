/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-04-06     imgcr       the first version
 */
#pragma once

#include <utilities/vel.hxx>
#include "colors.hxx"

class ColorVel: public Vel<Colors::Argb> {
    using Vel<Colors::Argb>::Vel;
protected:
    virtual float ease(float x) override;
    virtual Colors::Argb intepl(const Colors::Argb& originVal, const Colors::Argb& targetVal, float pos) override;
};

