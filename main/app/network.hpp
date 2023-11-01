#pragma once

#include "nic.hpp"
#include "mqtt.hpp"

#include "config.hpp"

using driver::ConfigDriver;
using driver::MqttDriver;
using driver::NetworkController;
using driver::NetworkEvent;

namespace app
{
    class NetworkManager
    {
    private:
        ConfigDriver& config_;
        NetworkController controller_;
        MqttDriver mqtt_;
    public:
        NetworkManager() : config_(ConfigDriver::instance()){};
        void init();

        void eventHandler(NetworkEvent event);
    };
}