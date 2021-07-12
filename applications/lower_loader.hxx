#pragma once
#include "loader.hxx"

class LowerLoader: public Loader {
public:
    virtual std::shared_ptr<App> createApp() override;
};