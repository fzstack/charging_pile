#include "ota_present.hxx"

OtaPresent::OtaPresent(std::shared_ptr<LoadingWidget> loading, std::shared_ptr<ProgressWidget> progress): loading(loading), progress(progress) {
    timer.onRun += [this, i = 0]() mutable {
        i++;
        i %= colors.size();
        this->progress->show(colors[i]);
    };
    timer.start();
}

void OtaPresent::showLoading(bool isShow) {
    loading->setType(LoadingWidget::Type::Info);
    if(isShow) {
        loading->show();
    } else {
        loading->hide();
    }
}

void OtaPresent::setProgress(int p) {
    progress->setProgress(p);
}