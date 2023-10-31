#pragma once

#include "mqtt_client.h"

#include <string>

namespace driver
{
    class MqttDriver
    {
    private:
        esp_mqtt_client_handle_t client_;

        void sendString(std::string data);

        uint8_t reconnect_attempts_ {0};

    public:
        bool init(std::string& url);
        void handleEvent(esp_event_base_t base, int32_t id, void *data);
    };
} // namespace driver