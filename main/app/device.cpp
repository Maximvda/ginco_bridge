#include "device.hpp"

/* esp includes */
#include "esp_log.h"

using app::Device;
using data::ConfigFunction;

static const char *TAG = {"Device"};

void Device::init()
{

}
void Device::handleConfig(GincoMessage &message)
{
    switch(message.function<ConfigFunction>())
    {
        case ConfigFunction::REQUEST_ADDRESS:
        {
            /* This sets the upper uint32_t value of can message to switchs id */
            message.data<uint32_t, true>(switches_.size()+1, 1);
            message.function<ConfigFunction>(ConfigFunction::SET_ADDRESS);
            ESP_LOGI(TAG, "giving address %u | %llu", switches_.size()+1, message.data<uint64_t>());
            message.send();
            break;
        }
        case ConfigFunction::HEARTBEAT:
        {
            ESP_LOGI(TAG, "heartbeat %u", message.source());
            break;
        }
        default:
        {
            ESP_LOGI(TAG, "received config function %u", message.function<uint8_t>());
            assert(0);
            break;
        }
    }
}