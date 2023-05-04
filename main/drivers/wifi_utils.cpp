#include "wifi_utils.h"

#include "esp_wifi.h"

#define WIFI_RETRY_COUNT 6
#define WIFI_RETRY_DELAY 500

esp_err_t wifi_utils::setMode(wifi_mode_t mode){
    esp_err_t result = esp_wifi_set_mode(mode);
    for ( int i=0 ; result != ESP_OK && i < WIFI_RETRY_COUNT ; i++){
        vTaskDelay(pdMS_TO_TICKS(WIFI_RETRY_DELAY));
        result = esp_wifi_set_mode(mode);
    }
    return result;
}

esp_err_t wifi_utils::getConfig(wifi_interface_t iface , wifi_config_t & wifi_config){
    esp_err_t result = esp_wifi_get_config( iface , &wifi_config);
    for ( int i=0 ; result != ESP_OK && i < WIFI_RETRY_COUNT ; i++){
        vTaskDelay(pdMS_TO_TICKS(WIFI_RETRY_DELAY));
        result = esp_wifi_get_config( iface ,&wifi_config);
    }
    return result;
}

esp_err_t wifi_utils::setConfig(wifi_interface_t iface ,  wifi_config_t &wifi_config ){
    esp_err_t result = esp_wifi_set_config( iface, &wifi_config);
    for ( int i=0 ; result != ESP_OK && i < WIFI_RETRY_COUNT ; i++){
        vTaskDelay(pdMS_TO_TICKS(WIFI_RETRY_DELAY));
        result = esp_wifi_set_config((wifi_interface_t) iface, &wifi_config);
    }
    return result;
}

esp_err_t wifi_utils::startWifi(){
    esp_err_t result = esp_wifi_start();
    for ( int i=0 ; result != ESP_OK && i < WIFI_RETRY_COUNT ; i++){
        vTaskDelay(pdMS_TO_TICKS(WIFI_RETRY_DELAY));
        result = esp_wifi_start();
    }
    return result;
}

esp_err_t wifi_utils::startWifiScan(const void *scan_config ){
    esp_err_t result = esp_wifi_scan_start((const wifi_scan_config_t *) scan_config, false);
    for ( int i=0 ; result != ESP_OK && i < WIFI_RETRY_COUNT ; i++){
        vTaskDelay(pdMS_TO_TICKS(WIFI_RETRY_DELAY));
        result = esp_wifi_scan_start((const wifi_scan_config_t *)scan_config, false);
    }
    return result;
}

esp_err_t wifi_utils::stopWifi(){
    esp_err_t result = esp_wifi_stop();
    for ( int i=0 ; result != ESP_OK && i < WIFI_RETRY_COUNT ; i++){
        vTaskDelay(pdMS_TO_TICKS(WIFI_RETRY_DELAY));
        result = esp_wifi_stop();
    }
    return result;
}

esp_err_t wifi_utils::connect(){
    esp_err_t result = esp_wifi_connect();
    for ( int i=0 ; result != ESP_OK && i < WIFI_RETRY_COUNT ; i++){
        vTaskDelay(pdMS_TO_TICKS(WIFI_RETRY_DELAY));
        result = esp_wifi_connect();
    }
    return result;
}
