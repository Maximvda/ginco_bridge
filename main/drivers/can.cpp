#include "can_driver.h"

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include "driver/twai.h"
#include "esp_log.h"

using namespace driver;

const static char* TAG = "can driver";
static can::message_t rec_message;
static can::message_t sent_message;
static TaskHandle_t can_task;
static void (*on_message_cb)(can::message_t message);

void can::init(void (*_callback)(can::message_t message)){
    on_message_cb = _callback;
    //Initialize configuration structures using macro initializers
    twai_general_config_t g_config = TWAI_GENERAL_CONFIG_DEFAULT(GPIO_NUM_5, GPIO_NUM_35, TWAI_MODE_NORMAL);
    twai_timing_config_t t_config = TWAI_TIMING_CONFIG_500KBITS();
    twai_filter_config_t f_config = TWAI_FILTER_CONFIG_ACCEPT_ALL();
    f_config.acceptance_code = 0x0;
    f_config.acceptance_mask = 0x3ffffff; // Don't accept events

    //Install CAN driver
    if (twai_driver_install(&g_config, &t_config, &f_config) == ESP_OK) {
        ESP_LOGI(TAG, "installed");
    } else {
        ESP_LOGI(TAG, "installation failed.");
        return;
    }

    //Start CAN driver
    if (twai_start() == ESP_OK) {
        ESP_LOGI(TAG, "started.");
        return;
    }
    ESP_LOGI(TAG, "failure starting");
}

bool can::transmit(can::message_t can_mes, bool blocking){
    //Configure message to transmit
    twai_message_t message;
    message.identifier = can_mes.get_can_id();
    message.extd = 1; // Extended ID
    message.data_length_code = can_mes.buffer_size;
    for (int i=0; i < can_mes.buffer_size; i++) {
        message.data[i] = static_cast<uint8_t>(can_mes.data >> (7-i));
    }
    bool result = (twai_transmit(&message, pdMS_TO_TICKS(1000)) == ESP_OK);
    if (result && blocking){
        can_task = xTaskGetCurrentTaskHandle();
        sent_message = can_mes; // Store sent message for acknowledgement comparison
        result = ulTaskNotifyTake(pdTRUE, pdMS_TO_TICKS(3)) == pdTRUE;
    }

    return result;


}

bool can::transmitFromProto(const Ginco__CanMessage message){
    can::message_t can_mes = {};
    can_mes.source_id = message.source_id;
    can_mes.event = message.event;
    can_mes.linked = message.linked;
    can_mes.ack = message.ack;
    can_mes.feature_type = message.feature_type;
    can_mes.index = message.index;
    can_mes.function_address = message.function_address;
    can_mes.buffer_size = message.buffer_size;
    can_mes.data = message.data;
    return can::transmit(can_mes);
}

void can::receive_task(void *pxptr){
    while (1){
        twai_message_t message;
        if (twai_receive(&message, pdMS_TO_TICKS(10000)) == ESP_OK){
            //rec_message.id = message.identifier;
            rec_message.source_id = (message.identifier >> 18) & 0xFF;
            rec_message.linked = (message.identifier >> 17) & 0x01;
            rec_message.ack = (message.identifier >> 16) & 0x01;
            rec_message.feature_type = (message.identifier >> 13) & 0x07;
            rec_message.index = (message.identifier >> 8) & 0x1F;
            rec_message.function_address = message.identifier & 0xFF;
            rec_message.buffer_size = message.data_length_code;
            rec_message.data = message.data[0];

            if (rec_message.isAcknowledge(sent_message)){
                ESP_LOGI(TAG, "Received ackownledgement frame");
                xTaskNotifyGive(can_task);
            }


            on_message_cb(rec_message);
        }
    }
}





