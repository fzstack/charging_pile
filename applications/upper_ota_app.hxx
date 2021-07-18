#pragma once
#include "app.hxx"
#include <string>
#include <string_view>
#include <ui/ota_present.hxx>
#include <devices/air724.hxx>
#include <applications/ota.hxx>
#include <components/handshake.hxx>
#include <components/watch_dog.hxx>
#include <components/http_client.hxx>
#include "loader_preset.hxx"

class UpperOtaApp: public App {
public:
    UpperOtaApp(std::string&& module, std::string&& version, std::string&& url, int size);
    virtual void run() override;
private:
    std::shared_ptr<OtaPresent> otaPresent = Preset::OtaPresent::get();
    std::shared_ptr<Air724> air724 = Preset::Air724::get();
    std::shared_ptr<Ota> ota = Preset::Ota::get();
    std::shared_ptr<Handshake> handshake = Preset::Handshake::get(HandshakeType::Ota);
    std::shared_ptr<WatchDog> watchDog = Preset::WatchDog::get();
    std::shared_ptr<UpperLoader> loader = Preset::Loader::get();

    std::string module, version, url;
    int size;

    std::shared_ptr<HttpClient> http;


};