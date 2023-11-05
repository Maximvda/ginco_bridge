#pragma once

#include "mqtt_client.h"
#include "upgrade.hpp"

#include <string>
#include <memory>

using component::UpgradeHandler;

namespace component
{
    class MqttClient
    {
    private:
        esp_mqtt_client_handle_t client_;
        uint8_t reconnect_attempts_ {0};
        std::unique_ptr<UpgradeHandler> upgrade_handler_ {nullptr};

        void handleMessage(esp_mqtt_event_handle_t event);
        void sendString(std::string data);


    public:
        bool init(std::string& url);
        void handleEvent(esp_event_base_t base, int32_t id, void *data);
    };
} // namespace driver