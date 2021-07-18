/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-04-08     imgcr       the first version
 */

#include "present.hxx"

Present::Present(std::shared_ptr<LoadingWidget> loading, std::shared_ptr<StateNotiWidget> stateNoti, std::shared_ptr<ProgressWidget> progress, std::shared_ptr<AppState> appState) {
    appState->cloudConnected.onChanged += [loading](auto value) {
        if(value) {
            loading->hide();
        } else {
            loading->setType(LoadingWidget::Type::Info);
            loading->show();
        }
    };

    appState->signal.onChanged += [loading, appState](auto value) {
        if(!*appState->cloudConnected) return;
        if(value < 12 || value == 99) {
            loading->setType(LoadingWidget::Type::Warn);
            loading->show();
        } else {
            loading->hide();
        }
    };
}

