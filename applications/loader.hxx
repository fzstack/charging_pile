#pragma once

#ifdef __cplusplus
extern "C" {
#endif

void runLoader();

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
#include "app.hxx"
#include <memory>
class Loader {
public:
    virtual std::shared_ptr<App> createApp() = 0;
};
#endif
