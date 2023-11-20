#include "can.hpp"

#include "esp_log.h"
#include "supervisor.hpp"

using driver::can::CanDriver;

const static char *TAG = "can driver";

void CanDriver::init(MessageCb cb_fnc)
{
    message_cb_ = cb_fnc;
    start();
}

void CanDriver::start()
{
    // Initialize configuration structures using macro initializers
    twai_general_config_t g_config = TWAI_GENERAL_CONFIG_DEFAULT(
        GPIO_NUM_5,
        GPIO_NUM_35,
        TWAI_MODE_NORMAL
    );
    twai_timing_config_t t_config = TWAI_TIMING_CONFIG_500KBITS();
    twai_filter_config_t f_config = {
        .acceptance_code = 0x0,
        .acceptance_mask = 0x3ffffff,
        .single_filter = false
    };
    g_config.intr_flags = ESP_INTR_FLAG_IRAM;

    // Install CAN driver
    if (twai_driver_install(&g_config, &t_config, &f_config) != ESP_OK)
    {
        ESP_LOGE(TAG, "install failed");
        return;
    }

    // Start CAN driver
    if (twai_start() != ESP_OK)
    {
        ESP_LOGE(TAG, "start failed");
        return;
    }
}

void CanDriver::tick()
{
    twai_message_t message;
    /* As long as frames are ready process them and aknowledge them! */
    while(twai_receive(&message, 50) == ESP_OK)
    {
        /* Frame received, always aknowledge! TODO: Optimise? */
        GincoMessage mes_data(message);
        /* Handle the message */
        message_cb_(mes_data);
    }
}

bool CanDriver::transmit(GincoMessage &can_mes, bool blocking)
{
    twai_message_t& transmit_mes = can_mes.message();
    esp_err_t res = twai_transmit(&transmit_mes, 50);
    if (blocking && res == ESP_OK)
    {
        twai_message_t message;
        if(twai_receive(&message, 1000) == ESP_OK)
        {
            return transmit_mes.identifier == (message.identifier ^ (1<<16));;
        }
        ESP_LOGW(TAG, "Failed to receive acknowledge!");
        return false;
    }
    if (res != ESP_OK)
    {
        ESP_LOGW(TAG, "transmit failed %i", res);
    }
    if (res == ESP_ERR_INVALID_STATE)
    {
        twai_stop();
        twai_driver_uninstall();
        start();
    }
    return res == ESP_OK;
}