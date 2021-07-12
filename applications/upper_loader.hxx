#pragma once
#include "loader.hxx"

class UpperLoader: public Loader {
public:
    virtual std::shared_ptr<App> createApp() override;
};