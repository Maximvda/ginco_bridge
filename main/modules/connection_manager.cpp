#include "connection_manager.h"

#include <stdio.h>
#include <atomic>
#include "string.h"

#include "esp_log.h"

#include "config_driver.h"
#include "ip_interface.h"
#include "event_list.h"
#include "socket_listener.h"

static const char *TAG = "Con manager";

using namespace ip_interface;

static void event_callback(SignaList event, eAdapterType type){
    ESP_LOGI(TAG, "Got event callback %i", event);
    switch (event){
        case SIGNAL_IPDRIVER_CONNECTED:
            if (type == eAdapterType::ADAPTER_AP){
                socket_listener::init();
            }
            break;
        default:
            break;
    }
}

void connection_manager::init(){
    bool configured = config::get_key(CONFIG_KEY_CONFIGURED) == 1;
    if (!configured){
        ESP_LOGW(TAG, "WiFi not yet configured, starting AP mode");
        ip_interface::init(eAdapterType::ADAPTER_AP, event_callback);
    }
    ip_interface::init(eAdapterType::ADAPTER_STA, event_callback);
}

void connection_manager::start(eAdapterType type){
    ip_interface::swap(type);
}
