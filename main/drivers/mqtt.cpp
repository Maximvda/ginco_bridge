#include "mqtt.hpp"

#include "esp_log.h"

using driver::MqttDriver;

static const char *TAG = "Mqtt";
void eventHandler(void *arg, esp_event_base_t base, int32_t id, void *data);

bool MqttDriver::init(std::string& url)
{
    esp_mqtt_client_config_t mqtt_cfg = {};
    mqtt_cfg.broker.address.uri = url.data();
    ESP_LOGI(TAG, "connecting to %s", mqtt_cfg.broker.address.uri);
    client_ = esp_mqtt_client_init(&mqtt_cfg);
    esp_mqtt_client_register_event(
        client_,
        static_cast<esp_mqtt_event_id_t>(ESP_EVENT_ANY_ID),
        eventHandler,
        this
    );
    assert(client_ != nullptr);
    return esp_mqtt_client_start(client_) == ESP_OK;

}

void MqttDriver::handleEvent(esp_event_base_t base, int32_t id, void *data)
{
    ESP_LOGI(TAG, "base=%s, event_id=%li", base, id);

    switch ((esp_mqtt_event_id_t)id)
    {
    case MQTT_EVENT_CONNECTED:
        ESP_LOGI(TAG, "MQTT_EVENT_CONNECTED");
        //TODO: Report connected!
        esp_mqtt_client_subscribe(client_, "ginco_bridge/command", 1);
        sendString("version: 0.0.1");
        break;
    case MQTT_EVENT_DISCONNECTED:
        ESP_LOGI(TAG, "MQTT_EVENT_DISCONNECTED");
        if(reconnect_attempts_++ > 3)
        {
            /*TODO: Go to AP mode for new config */
        }
        break;

    case MQTT_EVENT_SUBSCRIBED:
        // ESP_LOGI(TAG, "MQTT_EVENT_SUBSCRIBED, msg_id=%d", event->msg_id);
        break;
    case MQTT_EVENT_UNSUBSCRIBED:
        // ESP_LOGI(TAG, "MQTT_EVENT_UNSUBSCRIBED, msg_id=%d", event->msg_id);
        break;
    case MQTT_EVENT_PUBLISHED:
        // ESP_LOGI(TAG, "MQTT_EVENT_PUBLISHED, msg_id=%d", event->msg_id);
        break;

    case MQTT_EVENT_DATA:
    {
        //TODO: Handle event data
        // if (event->total_data_len == event->data_len)
        // {
        //     dispatch(reinterpret_cast<uint8_t *>(event->data), event->data_len);
        //     return;
        // }
        // process(*event);
        break;
    }
    case MQTT_EVENT_ERROR:
        ESP_LOGI(TAG, "MQTT_EVENT_ERROR");
        break;
    default:
        // ESP_LOGI(TAG, "Other event id:%d", event->event_id);
        break;
    }
}

void MqttDriver::sendString(std::string data)
{
    esp_mqtt_client_publish(client_, "ginco_bridge/status", data.data(), data.length(), 0, 0);
}

void eventHandler(void *arg, esp_event_base_t base, int32_t id, void *data)
{
    assert(arg != nullptr);
    MqttDriver* controller = reinterpret_cast<MqttDriver*>(arg);
    controller->handleEvent(base, id, data);
}