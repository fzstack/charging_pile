#include "lower_loader.hxx"
#include <rtthread.h>
#include "lower_major_app.hxx"
#include "lower_ota_app.hxx"

std::shared_ptr<App> LowerLoader::createApp() {
    //读取启动参数
    auto method = (Method)getStartMethod();
    auto app = std::shared_ptr<LowerApp>{};
    switch(method) {
    case Method::Major:
        app = std::make_shared<LowerMajorApp>();
        break;
    case Method::Ota:
        app = std::make_shared<LowerOtaApp>();
        break;
    default:
        rt_kprintf("unknown method\n");
        break;
    }

    if(app) {
        //受控模式切换
        auto handshake = app->getHandshake();
        handshake->onHandshake += [this, method](auto type) {
            auto targetMethod = handshakeTypeToMethod(type);
            if(targetMethod != method) {
                restart((int)targetMethod);
            }
        };
    }
    return app;
}

LowerLoader::Method LowerLoader::handshakeTypeToMethod(HandshakeType type) {
    switch(type) {
    case HandshakeType::Major:
        return Method::Major;
    case HandshakeType::Ota:
        return Method::Ota;
    default:
        return Method::Unknown;
    }
}