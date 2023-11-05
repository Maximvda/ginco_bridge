#include "can_thread.hpp"

#include "esp_log.h"
#include "ginco_types.hpp"
#include "supervisor.hpp"

using namespace app;
using data::Function;
using data::GincoMessage;
using utils::Message;

const static char *TAG = "can thread";

void CanTask::onStart()
{
    ESP_LOGI(TAG, "started.");
    can_driver.init(
        [this](const GincoMessage& mes){
            this->handleCanMes(mes);
        }
    );
}

void CanTask::tick()
{
    if (upgrading)
    {
        return;
    }
    can_driver.tick();
}

void CanTask::handle(Message &message)
{
    switch (message.event())
    {
    case EVENT_CAN_TRANSMIT:
    {
        if (auto mes = message.takeValue<GincoMessage>())
        {
            can_driver.transmit(*mes.get());
        }
        break;
    }
    default:
        assert(0);
    }
}

void CanTask::handleCanMes(const GincoMessage& message)
{
    app::taskFinder().ginco().frameReady(message);
}