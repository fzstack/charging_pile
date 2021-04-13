/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-04-13     imgcr       the first version
 */

#ifdef TEST_PROGRESS_WIDGET
#include <ui/progress_widget.hxx>
#include <rtthread.h>
#include <utilities/cmd.hxx>
#include <string>
using namespace std;

void test_progress(int argc, char** argv) {
    auto widget = Preset::ProgressWidget::get();
    Cmd{argc, argv}([&](Cmd& cmd){
        cmd.select({
            {"show", [&]{
                widget->show();
            }},
            {"hide", [&]{
                widget->hide();
            }},
            {"set", [&]{
                widget->setProgress(cmd.get<int>());
            }},
        });
    });
}

MSH_CMD_EXPORT(test_progress,);

#endif

