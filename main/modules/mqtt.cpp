#include "mqtt_module.h"

#include "wifi_driver.h"
#include "config_driver.h"
#include "esp_log.h"

static const char *TAG = "Mqtt";

void mqtt::init(){
    wifi_init();
    
    bool result {false};
    ESP_LOGI(TAG, "Starting setup of mqtt client");
    result = wifi_init_sta(
        config::get_string(WIFI_KEY, 0),
        config::get_string(WIFI_PASS, 0)
    );

    if (!result){
        ESP_LOGW(TAG, "No valid wifi credentials, starting ap mode");
    }
}