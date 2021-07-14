#include <ui/ws_screen.hxx>
#include <ui/rgb_light_widget.hxx>
#include <ui/loading_widget.hxx>
#include <ui/state_noti_widget.hxx>
#include <ui/progress_widget.hxx>

namespace Preset {
const int
    WsScreen::kWidth = 5,
    WsScreen::kHeight = 2;

const int
    LoadingWidget::zIndex = 2,
    ProgressWidget::zIndex = 1,
    StateNotiWidget::zIndex = 0;

}