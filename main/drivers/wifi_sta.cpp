#include "wifi_sta.h"

#include <atomic>
#include <string.h>

#include "esp_wifi.h"
#include "esp_event_loop.h"

#include "ip_interface.h"

class WiFiStaInterface : public IpInterface {
    private:
        wifi_config_t config;
        std::atomic_int failures;
        std::atomic_bool configPending;
        std::atomic_bool started;
    public:
        virtual void start();
        virtual void stop();
        virtual void handle(system_event_t* event);
};

void WiFiStaInterface::start(){
    failures = 0;
    int retries = 6;
    auto rc = esp_wifi_set_mode(WIFI_MODE_STA);
    while (rc == ESP_ERR_WIFI_STOP_STATE && retries--) {
        vTaskDelay(50);
        rc = esp_wifi_set_mode(WIFI_MODE_STA);
    }
    if (configPending){
        esp_wifi_set_config(WIFI_IF_STA, &config);
        configPending = false;
    }
    esp_wifi_start();
    started = true;
}

void WiFiStaInterface::stop(){
    started = false;
    ESP_ERROR_CHECK(esp_wifi_stop());
}
