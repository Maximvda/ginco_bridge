#pragma once

#include <memory>

#include "mqtt_client.h"

namespace component
{
    class Upgrader {
    public:
        virtual bool init(Ginco__Command* command) = 0;
        virtual bool receive(const uint8_t * data, uint32_t len) = 0;
        virtual void complete() = 0;
        virtual void fail() = 0;
        virtual ~Upgrader() {};
    };

    class UpgradeHandler:
    {
    private:
        std::unique_ptr<Upgrader> active_receiver {nullptr};
    public:
        UpgradeHandler(const esp_mqtt_event_t& event);
        void handle(const esp_mqtt_event_t& message);
        void complete();
    };

} // namespace driver