#include "ginco_thread.hpp"

#include "esp_log.h"
#include "can.hpp"

using app::GincoTask;
using utils::Message;
using data::FeatureType;

const static char *TAG = "ginco thread";

void GincoTask::onStart()
{
    device_.init();
    ESP_LOGI(TAG, "started.");
}

void GincoTask::onTimeout()
{
}

void GincoTask::handle(Message &message)
{
    switch (message.event())
    {
    case EVENT_CAN_RECEIVED:
    {
        if (auto mes = message.takeValue<GincoMessage>())
        {
            /*TODO: Handle can frame */
            /*TODO: CHECK IF FRAME IS FOR THIS DEVICE OR DIFFERENT DEVICE */
            auto& data = *mes.get();
            switch (data.feature())
            {
            case FeatureType::CONFIG:
            {
                device_.handleConfig(data);
                break;
            }
            default:
                break;
            }
        }
        break;
    }
    default:
        assert(0);
    }
}
