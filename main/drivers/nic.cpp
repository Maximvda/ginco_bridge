#include "nic.hpp"

#include "string.h"
#include "esp_log.h"

static const char *TAG = "Nic";

using driver::NetworkController;
using driver::NetworkAdapter;
using driver::WifiStaI;
using driver::WifiApI;
using driver::IpInterface;

static void eventHandler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data);

void NetworkController::init(NetworkAdapter adapter)
{
    ESP_ERROR_CHECK(esp_netif_init());
    const wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));
    ESP_ERROR_CHECK(esp_wifi_get_config(WIFI_IF_AP, &wifi_config_));
    for (auto* interface : interfaces_)
    {
        interface->init(wifi_config_);
    }
    ESP_ERROR_CHECK(esp_event_handler_instance_register(
        WIFI_EVENT,
        ESP_EVENT_ANY_ID,
        &eventHandler,
        this,
        NULL
    ));
    active_interface_ = *interfaces_[static_cast<uint8_t>(adapter)];
    active_interface_.start();
}

void NetworkController::handleEvent(esp_event_base_t event, int32_t id, void* data)
{
    active_interface_.handleEvent(id, data);
}

void NetworkController::setSsid(const char* ssid, const char* pass)
{
    strcpy((char *)wifi_config_.sta.ssid, ssid);
    strcpy((char *)wifi_config_.sta.password, pass);
    sta_i_.config_pending_ = true;
}

void WifiStaI::init(wifi_config_t& config)
{
    esp_netif_create_default_wifi_sta();
    if(config_pending_)
    {
        ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &config));
    }
    config_pending_ = false;
    ESP_LOGI(TAG, "WiFi STA done.");
}

void WifiApI::init(wifi_config_t& config)
{
    esp_netif_create_default_wifi_ap();
    config.ap.authmode = WIFI_AUTH_OPEN;
    if (config.ap.ssid[0] == 0)
    {
        sprintf((char *)config.ap.ssid, "ginco_bridge");
        ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_AP, &config));
    }
    ESP_LOGI(TAG, "WiFi AP init done.");
}

void WifiStaI::handleEvent(int32_t id, void* data)
{
    ESP_LOGI(TAG, "Client event %lu", id);
    switch (id)
    {
    case WIFI_EVENT_STA_START:
        // add_event(ip_type, SIGNAL_IPDRIVER_RUNNING);
        // if (esp_wifi_connect() == ESP_ERR_WIFI_SSID)
            // add_event(ip_type, SIGNAL_IPDRIVER_CONNECTION_ERROR);
        break;
    case WIFI_EVENT_STA_CONNECTED:
        // add_event(ip_type, SIGNAL_IPDRIVER_CONNECTED);
        break;
    case WIFI_EVENT_STA_DISCONNECTED:
    {
        // add_event(ip_type, SIGNAL_IPDRIVER_DISCONNECTED);
        wifi_event_sta_disconnected_t *event = (wifi_event_sta_disconnected_t *)data;
        ESP_LOGI(TAG, "WiFi reason: %u", event->reason);
        // handleDisconnect(static_cast<wifi_err_reason_t>(event->reason));
        break;
    }
    default:
        break;
    }
}

void WifiApI::handleEvent(int32_t id, void* data)
{
    switch (id)
    {
    case WIFI_EVENT_AP_START:
        // add_event(ip_type, SIGNAL_IPDRIVER_RUNNING);
        break;
    case WIFI_EVENT_AP_STACONNECTED:
    {
        wifi_event_ap_staconnected_t *event = (wifi_event_ap_staconnected_t *)data;
        // ESP_LOGI(TAG, "station " MACSTR " join, AID=%d", MAC2STR(event->mac), event->aid);
        // add_event(ip_type, SIGNAL_IPDRIVER_CONNECTED);
        break;
    }
    case WIFI_EVENT_AP_STADISCONNECTED:
    {
        wifi_event_ap_stadisconnected_t *event = (wifi_event_ap_stadisconnected_t *)data;
        // ESP_LOGI(TAG, "station " MACSTR " leave, AID=%d", MAC2STR(event->mac), event->aid);
        break;
    }
    default:
        break;
    }
}

void IpInterface::start()
{
    esp_wifi_stop();
    wifi_mode_t mode = adapter_ == NetworkAdapter::AP ? WIFI_MODE_AP : WIFI_MODE_STA;
    ESP_ERROR_CHECK(esp_wifi_set_mode(mode));
    ESP_ERROR_CHECK(esp_wifi_start());
    ESP_LOGI(TAG, "%s started.", name());
}

static void eventHandler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data)
{
    if (arg == nullptr)
        return;
    NetworkController* controller = reinterpret_cast<NetworkController*>(arg);
    controller->handleEvent(event_base, event_id, event_data);
}

// void WiFiStaInterface::handleDisconnect(wifi_err_reason_t reason)
// {
//     switch (reason)
//     {
//     case WIFI_REASON_BEACON_TIMEOUT:
//     case WIFI_REASON_NO_AP_FOUND:
//     case WIFI_REASON_AUTH_FAIL:
//     case WIFI_REASON_ASSOC_FAIL:
//     case WIFI_REASON_HANDSHAKE_TIMEOUT:
//     case WIFI_REASON_CONNECTION_FAIL:
//     case WIFI_REASON_AP_TSF_RESET:
//     case WIFI_REASON_ROAMING:
//     case WIFI_REASON_4WAY_HANDSHAKE_TIMEOUT:
//     case WIFI_REASON_ASSOC_COMEBACK_TIME_TOO_LONG:
//         add_event(ip_type, SIGNAL_IPDRIVER_CONNECTION_ERROR);
//         break;
//     default:
//         break;
//     }
// }