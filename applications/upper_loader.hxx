#pragma once
#include "loader.hxx"

class UpperLoader: public Loader {
public:
    UpperLoader();
    virtual std::shared_ptr<App> createApp() override;
    void ota(std::string&& module, std::string&& version, std::string&& url, int size);
    void major();
private:
    enum class Method {
        Major,
        Ota,
    };
};