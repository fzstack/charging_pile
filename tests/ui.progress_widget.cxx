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

