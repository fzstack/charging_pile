/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-04-07     imgcr       the first version
 */
#pragma once
#include "loading_widget.hxx"
#include "state_noti_widget.hxx"
#include <components/app_state.hxx>

class Present {
public:
    Present(std::shared_ptr<LoadingWidget> loading, std::shared_ptr<StateNotiWidget> stateNoti, std::shared_ptr<AppState> appState);
};

#include <utilities/singleton.hxx>
namespace Preset {
class Present: public Singleton<Present>, public ::Present {
    friend singleton_t;
    Present(): ::Present(LoadingWidget::get(), StateNotiWidget::get(), AppState::get()) {

    }
};
}


