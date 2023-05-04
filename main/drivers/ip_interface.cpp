#include "ip_interface.h"

#include "string.h"
#include "esp_wifi.h"
#include "esp_log.h"
#include "esp_mac.h"
#include "config_driver.h"

static const char *TAG = "Ip interface";

class WiFiApInterface : public IpInterface {
private:
    wifi_config_t config;
public:
    virtual void start();
    virtual void swap();
    virtual void event_handler(int32_t event_id, void* event_data);
};

class WiFiStaInterface : public IpInterface {
private:
    wifi_config_t config;
    void handleDisconnect(wifi_err_reason_t reason);
public:
    virtual void start();
    virtual void swap();
    virtual void event_handler(int32_t event_id, void* event_data);
};

static WiFiApInterface wifiInterface;
static WiFiStaInterface wifiStaInterface;
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
    ESP_ERROR_CHECK(esp_event_loop_create_default());

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
        esp_netif_create_default_wifi_sta();
    }

    activeHandler->start();
}

void ip_interface::swap(eAdapterType type){
    if (type == eAdapterType::ADAPTER_AP){
        activeHandler = ipInterfaces[0];
    } else {
        activeHandler = ipInterfaces[1];
    }

    activeHandler->swap();
}

void IpInterface::add_event(eAdapterType type, SignaList event){
    if (driver_callback != nullptr){
        return driver_callback(event, type);
    }
}


void WiFiApInterface::start(){
    ip_type = eAdapterType::ADAPTER_AP;
    esp_netif_create_default_wifi_ap();
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    esp_wifi_get_config(WIFI_IF_AP, &config);
    config.ap.authmode = WIFI_AUTH_OPEN;
    sprintf((char*) config.ap.ssid, "ginco_bridge");

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_AP));
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_AP, &config));
    ESP_ERROR_CHECK(esp_wifi_start());

    ESP_LOGI(TAG, "WiFi AP init done.");
};

void WiFiApInterface::swap(){
    //esp_wifi_stop();
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_AP));
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_AP, &config));
    ESP_ERROR_CHECK(esp_wifi_start());

    ESP_LOGI(TAG, "WiFi AP swap done.");
}

void WiFiApInterface::event_handler(int32_t event_id, void* event_data){
    switch(event_id){
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

void WiFiStaInterface::start(){
    ip_type = eAdapterType::ADAPTER_STA;
    esp_netif_create_default_wifi_sta();
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    esp_wifi_get_config(WIFI_IF_STA, &config);
    sprintf((char*) config.sta.ssid, config::get_string(CONFIG_KEY_SSID, 0));
    sprintf((char*) config.sta.password, config::get_string(CONFIG_KEY_PASS, 0));

    ESP_LOGI(TAG, "SSID: %s", (char*) config.sta.ssid);

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &config));
    ESP_ERROR_CHECK(esp_wifi_start());

    ESP_LOGI(TAG, "WiFi STA done.");
};

void WiFiStaInterface::swap(){
    esp_wifi_get_config(WIFI_IF_STA, &config);
    sprintf((char*) config.sta.ssid, config::get_string(CONFIG_KEY_SSID, 0));
    sprintf((char*) config.sta.password, config::get_string(CONFIG_KEY_PASS, 0));

    ESP_LOGI(TAG, "SSID: %s", (char*) config.sta.ssid);
    ESP_LOGI(TAG, "pass: %s", (char*) config.sta.password);
    //esp_wifi_stop();
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &config));
    ESP_ERROR_CHECK(esp_wifi_start());
}

void WiFiStaInterface::event_handler(int32_t event_id, void* event_data){
    ESP_LOGI(TAG, "Client event %lu", event_id);
    switch(event_id){
        case WIFI_EVENT_STA_START:
            esp_wifi_connect();
            break;
        case WIFI_EVENT_STA_CONNECTED:
            add_event(eAdapterType::ADAPTER_STA, SIGNAL_IPDRIVER_CONNECTED);
            break;
        case WIFI_EVENT_STA_DISCONNECTED:
            add_event(eAdapterType::ADAPTER_STA, SIGNAL_IPDRIVER_DISCONNECTED);
            wifi_event_sta_disconnected_t* event = (wifi_event_sta_disconnected_t*) event_data;
            handleDisconnect(event->reason);
            break;
        default:
            break;
    }
}

void WiFiStaInterface::handleDisconnect(wifi_err_reason_t reason){

}