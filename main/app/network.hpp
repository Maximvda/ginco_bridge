#pragma once

#include "nic.hpp"
#include "mqtt.hpp"

#include "config.hpp"

using driver::ConfigDriver;
using driver::NetworkController;
using driver::NetworkEvent;
using driver::NetworkAdapter;
using component::MqttClient;

namespace app
{
    class NetworkManager
    {
    private:
        ConfigDriver& config_;
        MqttClient mqtt_;
    public:
        NetworkController controller;

        NetworkManager() : config_(ConfigDriver::instance()){};
        void init();

        void eventHandler(NetworkEvent event);
        void startInterface(NetworkAdapter adapter);
    };
}