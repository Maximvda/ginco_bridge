#include "mqtt_module.h"

#include "config_driver.h"
#include "esp_log.h"
#include "ginco.pb-c.h"
#include "upgrade.h"

static const char *TAG = "Mqtt";

static esp_mqtt_client_handle_t mqtt_client;
static bool connected {false};
static upgrade::Handler upgrade_handler;

void reset_connection();
static bool dispatch(uint8_t * ptr, int length);
static void process(esp_mqtt_event_t& event);

bool mqtt::init(){
    esp_mqtt_client_config_t config = {};
    char* mqtt = config::get_string(CONFIG_KEY_MQTT);
    if(mqtt){
        return mqtt::set_url(mqtt);
    }
    return false;
}

static bool dispatchRequest(Ginco__Command* command){
    switch(command->command_case){
        case GINCO__COMMAND__COMMAND_UPGRADE:
            ESP_LOGI(TAG, "Upgrade received");
            break;
        default:
            break;
    }
    return true;
}

void mqtt_event_handler(void *handler_args, esp_event_base_t base,int32_t event_id, void *event_data){
    ESP_LOGD(TAG, "Event dispatched from event loop base=%s, event_id=%li", base, event_id);
    esp_mqtt_event_handle_t event = (esp_mqtt_event_handle_t) event_data;
    esp_mqtt_client_handle_t client = event->client;
    switch ((esp_mqtt_event_id_t)event_id)
    {
    case MQTT_EVENT_CONNECTED:
        ESP_LOGI(TAG, "MQTT_EVENT_CONNECTED");
        connected = true;
        esp_mqtt_client_subscribe(client, "ginco_bridge/command", 1);
        break;
    case MQTT_EVENT_DISCONNECTED:
        ESP_LOGI(TAG, "MQTT_EVENT_DISCONNECTED");
        connected = false;
        break;

    case MQTT_EVENT_SUBSCRIBED:
        ESP_LOGI(TAG, "MQTT_EVENT_SUBSCRIBED, msg_id=%d", event->msg_id);
        break;
    case MQTT_EVENT_UNSUBSCRIBED:
        ESP_LOGI(TAG, "MQTT_EVENT_UNSUBSCRIBED, msg_id=%d", event->msg_id);
        break;
    case MQTT_EVENT_PUBLISHED:
        ESP_LOGI(TAG, "MQTT_EVENT_PUBLISHED, msg_id=%d", event->msg_id);
        break;

    case MQTT_EVENT_DATA:{
        if (event->total_data_len == event->data_len) {
            dispatch(reinterpret_cast<uint8_t *> (event->data), event->data_len);
            return;
        }
        process(*event);
        break;
    }
    case MQTT_EVENT_ERROR:
        ESP_LOGI(TAG, "MQTT_EVENT_ERROR");
        break;
    default:
        ESP_LOGI(TAG, "Other event id:%d", event->event_id);
        break;
    }
}

bool mqtt::set_url(char* url){
    reset_connection();
    if(url == NULL || strlen(url) == 0){
        return false;
    }
    char client_id[19] = "ginco_bridge";
    esp_mqtt_client_config_t mqtt_cfg = {};
    mqtt_cfg.broker.address.uri = url;
    mqtt_cfg.credentials.client_id = client_id;
    mqtt_client = esp_mqtt_client_init(&mqtt_cfg);
    esp_mqtt_client_register_event(
        mqtt_client,
        static_cast<esp_mqtt_event_id_t>(ESP_EVENT_ANY_ID),
        mqtt_event_handler,
        NULL);
    if(mqtt_client == NULL){
        ESP_LOGE(TAG, "Failed to initialise mqtt client");
        return false;
    }
    return esp_mqtt_client_start(mqtt_client) == ESP_OK;
}

void reset_connection(){
    if(!mqtt_client)
        return;
    ESP_ERROR_CHECK(esp_mqtt_client_destroy(mqtt_client));
    connected = false;
    mqtt_client = NULL;
}

static bool dispatch(uint8_t * ptr, int length) {
    if (!length)
        return false;

    Ginco__Command * command = ginco__command__unpack(NULL, length, ptr);
    if(!command){
        ESP_LOGI(TAG, "Invalid command received");
        return false;
    }

    if (dispatchRequest(command)) {
        ginco__command__free_unpacked(command, NULL);
    }
    return true;
}

static void process(esp_mqtt_event_t& event){
	if (event.current_data_offset == 0) {
        upgrade_handler.init(event);
        return;
	}

    upgrade_handler.handle(event);
	if (event.current_data_offset + event.data_len == event.total_data_len) {
		upgrade_handler.end();
	}
}