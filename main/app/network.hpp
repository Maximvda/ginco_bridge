#pragma once

#include "nic.hpp"

#include "config.hpp"

using driver::ConfigDriver;

namespace app
{
    class NetworkManager
    {
    private:
        ConfigDriver& config_;
        driver::NetworkController controller_;
    public:
        NetworkManager() : config_(ConfigDriver::instance()){};
        void init();
    };
}