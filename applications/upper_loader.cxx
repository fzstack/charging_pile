#include "upper_loader.hxx"
#include "upper_app.hxx"
#include "upper_ota_app.hxx"
#include <components/persistent_storage_preset.hxx>
#include <config/app.hxx>

using namespace std;

UpperLoader::UpperLoader() {

}

std::shared_ptr<App> UpperLoader::createApp() {
    auto method = (Method)getStartMethod();
    switch(method) {
    case Method::Major:
        return std::make_shared<UpperApp>();
    case Method::Ota: {
         auto storage = Preset::PersistentStorage::get();
         auto params = storage->read<Config::Loader::OtaParams>();
         return std::make_shared<UpperOtaApp>(
             move(params.module),
             move(params.version),
             move(params.url),
             params.size
         );
         break;
    }
    default:
        rt_kprintf("unknown method\n");
        break;
    }
    return nullptr;
}

void UpperLoader::ota(std::string&& module, std::string&& version, std::string&& url, int size) {
    auto storage = Preset::PersistentStorage::get();
    auto params = Config::Loader::OtaParams {
        module: string{module},
        version: string{version},
        url: string{url},
        size: size,
    };
    storage->write(params);
    restart((int)Method::Ota);
}

void UpperLoader::major() {
    restart((int)Method::Major);
}