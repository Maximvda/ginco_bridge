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
    // switch(message.getFunction<ConfigFunction>())
    // {
    //     case ConfigFunction::REQUEST_ADDRESS:
    //     {
    //         message.setUpperValue(switches_.size()+1);
    //         message.setFunction<ConfigFunction>(ConfigFunction::SET_ADDRESS);
    //         ESP_LOGI(TAG, "giving address %u | %u | %lu", switches_.size()+1, message.data_length, message.value());
    //         message.send();
    //         break;
    //     }
    //     case ConfigFunction::HEARTBEAT:
    //     {
    //         ESP_LOGI(TAG, "heartbeat %u", message.source_id);
    //         break;
    //     }
    //     default:
    //     {
    //         ESP_LOGI(TAG, "received config function %u", static_cast<uint8_t>(message.getFunction<ConfigFunction>()));
    //         assert(0);
    //         break;
    //     }
    // }
}