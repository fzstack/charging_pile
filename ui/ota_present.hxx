#pragma once
#include "loading_widget.hxx"
#include "progress_widget.hxx"
#include <vector>
#include "colors.hxx"
#include <components/timer.hxx>

class OtaPresent {
public:
    OtaPresent(std::shared_ptr<LoadingWidget> loading, std::shared_ptr<ProgressWidget> progress);
    void showLoading(bool isShow);
    void setProgress(int progress);

private:
    std::shared_ptr<LoadingWidget> loading;
    std::shared_ptr<ProgressWidget> progress;
    std::vector<Colors::Argb> colors = {
        {90, 100, 0, 200},
        {90, 240, 0, 86},
    };
    Timer timer = {2000, "op"};
};

#include <utilities/singleton.hxx>
namespace Preset {
class OtaPresent: public Singleton<OtaPresent>, public ::OtaPresent {
    friend singleton_t;
    OtaPresent(): ::OtaPresent(LoadingWidget::get(), ProgressWidget::get()) {

    }
};
}

