#include "connection_manager.h"

#include <stdio.h>
#include <atomic>
#include "string.h"

#include "esp_log.h"

#include "config_driver.h"
#include "ip_interface.h"
#include "event_list.h"
#include "http_client.h"

static const char *TAG = "Con manager";

using namespace ip_interface;

static void event_callback(SignaList event, eAdapterType type){
    ESP_LOGI(TAG, "Got event callback %i", event);
    switch (event){
        case SIGNAL_IPDRIVER_CONNECTED:
            if (type == eAdapterType::ADAPTER_AP){
                http_client::init();
            }
            break;
        default:
            break;
    }
}

void connection_manager::init(){
    char* ssid;
    size_t length {0};
    ssid = config::get_string(CONFIG_KEY_SSID, length);
    if (length == 0){
        ESP_LOGW(TAG, "WiFi not yet configured, starting AP mode");
        ip_interface::init(eAdapterType::ADAPTER_AP, event_callback);
    }
}

void connection_manager::start(eAdapterType type){
    ip_interface::swap(type);
}
