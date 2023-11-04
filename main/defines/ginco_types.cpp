#include "ginco_types.hpp"

#include "esp_log.h"

#include "supervisor.hpp"

static constexpr char* TAG = "types";

using data::GincoMessage;

bool GincoMessage::acknowledge()
{
    ack_ = true;
    return app::taskFinder().can().transmit(*this);
}

bool GincoMessage::send(bool acknowledge)
{
    /* Block calling thread untill operation is finished */
    bool res = false;
    auto& can_task = app::taskFinder().can();
    if (acknowledge)
    {
        do {
            can_task.inspect(
                [&res, this](){res = app::taskFinder().can().can_driver.transmit(*this, true);}
            );
            if (!res)
            {
                ESP_LOGW(TAG, "Did retransmit!");
            }
        } while (!res); /* This will handle retransmits */
        return res;
    }
    return can_task.transmit(*this);
}