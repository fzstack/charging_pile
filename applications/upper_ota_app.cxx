#include "upper_ota_app.hxx"
#include <components/air_components.hxx>
#include "loader_preset.hxx"

UpperOtaApp::UpperOtaApp(std::string&& module, std::string&& version, std::string&& url, int size): module(module), version(version), url(url), size(size) {
    
    
}

void UpperOtaApp::run() {
    ota->onProgress += [this](auto module, auto value) {
        otaPresent->setProgress(value);
    };

    ota->onDone += [this](auto module) {
        // TODO: switch to major app
        loader->major();
    };

    ota->onError += [this](auto module, auto error, auto desc) {
        //TODO: reboot?
    };

    //TODO: if error
    //TODO: check if module is lower

    //让lower也进入ota模式
    //TODO: 在MAJOR模式的APP跳转到OTA模式(由LowerLoader来完成这个)

    rt_kprintf("module: %s\n", module.data());
    rt_kprintf("version: %s\n", version.data());
    rt_kprintf("url: \n");
    rt_kputs(url.data());
    rt_kputs("\n");

    rt_kprintf("handshaking...\n");
    handshake->hello();

    //初始化air724
    air724->init();

    http = air724->make<HttpClient>();
    auto stream = http->stream(
        std::make_shared<HttpRequest>()
        ->setUrl(url)
        ->setMethod(HttpMethod::Get)
    );

    otaPresent->showLoading(false);
    ota->start(version, module, stream, size);
    //TODO: lower也要有
}