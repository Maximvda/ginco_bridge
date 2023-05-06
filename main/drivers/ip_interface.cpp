#include "ip_interface.h"

#include <atomic>
#include "string.h"
#include "esp_wifi.h"
#include "esp_log.h"
#include "esp_mac.h"
#include "config_driver.h"

static const char *TAG = "Ip interface";

class WiFiApInterface : public IpInterface {
private:
public:
    virtual void init();
    virtual void start();
    virtual void event_handler(int32_t event_id, void* event_data);
};

class WiFiStaInterface : public IpInterface {
private:
    std::atomic_bool config_pending {false};
    void handleDisconnect(wifi_err_reason_t reason);
public:
    virtual void init();
    virtual void start();
    virtual void event_handler(int32_t event_id, void* event_data);
    void setSsid(const char* ssid, const char* pass);
};

static WiFiApInterface wifiInterface;
static WiFiStaInterface wifiStaInterface;
static wifi_config_t wifi_config;
static IpInterface* const ipInterfaces[] = {&wifiInterface, &wifiStaInterface};
static IpInterface* activeHandler;
static ip_interface::IpDriverCallback driver_callback = nullptr;

using namespace ip_interface;

static void wifi_event_handler(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data){
    if (event_base == IP_EVENT){
        ESP_LOGI(TAG, "ip event id: %li", event_id);
        if (event_id == IP_EVENT_STA_GOT_IP){
            ip_event_got_ip_t* event = (ip_event_got_ip_t*) event_data;
            ESP_LOGI(TAG, "got ip:" IPSTR, IP2STR(&event->ip_info.ip));
        }
        return;
    }
    activeHandler->event_handler(event_id, event_data);

}

void ip_interface::init(eAdapterType type, IpDriverCallback cb){
    driver_callback = cb;
    ESP_ERROR_CHECK(esp_netif_init());
    const wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));
    ESP_ERROR_CHECK(esp_wifi_get_config(WIFI_IF_AP, &wifi_config));
    ipInterfaces[0]->init();
    ipInterfaces[1]->init();

    ESP_ERROR_CHECK(
        esp_event_handler_instance_register(
            WIFI_EVENT,
            ESP_EVENT_ANY_ID,
            &wifi_event_handler,
            NULL,
            NULL)
    );
    esp_event_handler_instance_t instance_got_ip;
    ESP_ERROR_CHECK(
        esp_event_handler_instance_register(
            IP_EVENT,
            IP_EVENT_STA_GOT_IP,
            &wifi_event_handler,
            NULL,
            &instance_got_ip)
    );

    if (type == eAdapterType::ADAPTER_AP){
        activeHandler = ipInterfaces[0];
    } else {
        activeHandler = ipInterfaces[1];
    }

    activeHandler->start();
}

void ip_interface::start(eAdapterType type){
    if (type == eAdapterType::ADAPTER_AP){
        activeHandler = ipInterfaces[0];
    } else {
        activeHandler = ipInterfaces[1];
    }
    activeHandler->start();
}

void IpInterface::add_event(eAdapterType type, SignaList event){
    if (driver_callback != nullptr){
        return driver_callback(event, type);
    }
}

void WiFiApInterface::init(){
    ip_type = eAdapterType::ADAPTER_AP;
    esp_netif_create_default_wifi_ap();
    wifi_config.ap.authmode = WIFI_AUTH_OPEN;
    if (wifi_config.ap.ssid[0] == 0){
        sprintf((char*) wifi_config.ap.ssid, "ginco_bridge");
        ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_AP, &wifi_config));
    }

    ESP_LOGI(TAG, "WiFi AP init done.");
};

void WiFiApInterface::start(){
    esp_wifi_stop();
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_AP));
    ESP_ERROR_CHECK(esp_wifi_start());
    ESP_LOGI(TAG, "WiFi AP started.");
}

void WiFiApInterface::event_handler(int32_t event_id, void* event_data){
    switch(event_id){
        case WIFI_EVENT_AP_START:
            add_event(ip_type, SIGNAL_IPDRIVER_RUNNING);
            break;
        case WIFI_EVENT_AP_STACONNECTED:{
            wifi_event_ap_staconnected_t* event = (wifi_event_ap_staconnected_t*) event_data;
            ESP_LOGI(TAG, "station "MACSTR" join, AID=%d", MAC2STR(event->mac), event->aid);
            add_event(ip_type, SIGNAL_IPDRIVER_CONNECTED);
            break;
        }
        case WIFI_EVENT_AP_STADISCONNECTED:{
            wifi_event_ap_stadisconnected_t* event = (wifi_event_ap_stadisconnected_t*) event_data;
            ESP_LOGI(TAG, "station "MACSTR" leave, AID=%d",MAC2STR(event->mac), event->aid);
            break;
        }
        default:
            break;
    }
}

void WiFiStaInterface::init(){
    ip_type = eAdapterType::ADAPTER_STA;
    esp_netif_create_default_wifi_sta();
    if(config_pending){
        ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config));
        config_pending = false;
    }

    ESP_LOGI(TAG, "WiFi STA done.");
};

void WiFiStaInterface::start(){
    esp_wifi_stop();
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_start());
    ESP_LOGI(TAG, "WiFi STA started.");
}

void WiFiStaInterface::event_handler(int32_t event_id, void* event_data){
    ESP_LOGI(TAG, "Client event %lu", event_id);
    switch(event_id){
        case WIFI_EVENT_STA_START:
            add_event(ip_type, SIGNAL_IPDRIVER_RUNNING);
            if (esp_wifi_connect() == ESP_ERR_WIFI_SSID)
                add_event(ip_type, SIGNAL_IPDRIVER_CONNECTION_ERROR);
            break;
        case WIFI_EVENT_STA_CONNECTED:
            add_event(ip_type, SIGNAL_IPDRIVER_CONNECTED);
            break;
        case WIFI_EVENT_STA_DISCONNECTED:{
            add_event(ip_type, SIGNAL_IPDRIVER_DISCONNECTED);
            wifi_event_sta_disconnected_t* event = (wifi_event_sta_disconnected_t*) event_data;
            ESP_LOGI(TAG, "WiFi reason: %u", event->reason);
            handleDisconnect(static_cast<wifi_err_reason_t>(event->reason));
            break;
        }
        default:
            break;
    }
}

void WiFiStaInterface::handleDisconnect(wifi_err_reason_t reason){
    switch(reason){
        case WIFI_REASON_BEACON_TIMEOUT:
        case WIFI_REASON_NO_AP_FOUND:
        case WIFI_REASON_AUTH_FAIL:
        case WIFI_REASON_ASSOC_FAIL:
        case WIFI_REASON_HANDSHAKE_TIMEOUT:
        case WIFI_REASON_CONNECTION_FAIL:
        case WIFI_REASON_AP_TSF_RESET:
        case WIFI_REASON_ROAMING:
        case WIFI_REASON_4WAY_HANDSHAKE_TIMEOUT:
        case WIFI_REASON_ASSOC_COMEBACK_TIME_TOO_LONG:
            add_event(ip_type, SIGNAL_IPDRIVER_CONNECTION_ERROR);
            break;
        default:
            break;
    }
}

void ip_interface::setSsidPass(char* ssid, char* pass){
    wifiStaInterface.setSsid(ssid, pass);
}

void WiFiStaInterface::setSsid(const char * ssid, const char * pass) {
    ESP_ERROR_CHECK(esp_wifi_get_config(WIFI_IF_STA, &wifi_config));
    strcpy( (char  *) wifi_config.sta.ssid, ssid);
    strcpy( (char  *) wifi_config.sta.password, pass);
    config_pending = true;
}
