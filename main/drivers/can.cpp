#include "can.hpp"

#include "esp_log.h"
#include "supervisor.hpp"

using driver::can::CanDriver;

const static char *TAG = "can driver";

void CanDriver::init()
{
    // Initialize configuration structures using macro initializers
    twai_general_config_t g_config = TWAI_GENERAL_CONFIG_DEFAULT(
        GPIO_NUM_5,
        GPIO_NUM_35,
        TWAI_MODE_NORMAL);
    twai_timing_config_t t_config = TWAI_TIMING_CONFIG_500KBITS();
    twai_filter_config_t f_config = TWAI_FILTER_CONFIG_ACCEPT_ALL();
    f_config.acceptance_code = 0x0;
    f_config.acceptance_mask = 0x3ffffff; // Don't accept events

    // Install CAN driver
    if (twai_driver_install(&g_config, &t_config, &f_config) == ESP_OK)
    {
        ESP_LOGI(TAG, "installed");
    }
    else
    {
        ESP_LOGI(TAG, "installation failed.");
        return;
    }

    // Start CAN driver
    if (twai_start() == ESP_OK)
    {
        ESP_LOGI(TAG, "started.");
        return;
    }
    ESP_LOGI(TAG, "failure starting");
}

bool CanDriver::transmit(const GincoMessage &can_mes)
{
    last_message_ = can_mes;
    esp_err_t res = twai_transmit(&last_message_.canMessage(), 0);
    ESP_LOGI(TAG, "Response from transmit %d", res);
    return res == ESP_OK;
}

void CanDriver::tick()
{
    twai_message_t message;
    if (!twai_receive(&message, 0) == ESP_OK)
    {
        /* No frame received */
        return;
    }
    GincoMessage ginco_mes(message);
    ESP_LOGI(TAG, "frame rec: %u : %u", ginco_mes.source_id_, static_cast<uint>(ginco_mes.function));
    /* Frame received so send to correct place */
    app::taskFinder().ginco().frameReady(message);
}
