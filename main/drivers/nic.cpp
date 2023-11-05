#include "nic.hpp"

#include "string.h"
#include "esp_log.h"

#include "supervisor.hpp"
#include "socket.hpp"

static const char *TAG = "Nic";

using driver::NetworkController;
using driver::NetworkAdapter;
using driver::WifiStaI;
using driver::WifiApI;
using driver::IpInterface;

static void eventHandler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data);

void NetworkController::init(const NetworkAdapter& adapter)
{
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    const wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    for (auto& interface : interfaces_)
    {
        interface->init();
    }

    ESP_ERROR_CHECK(esp_event_handler_instance_register(
        WIFI_EVENT,
        ESP_EVENT_ANY_ID,
        &eventHandler,
        this,
        NULL
    ));
    ESP_ERROR_CHECK(esp_event_handler_instance_register(
        IP_EVENT,
        IP_EVENT_STA_GOT_IP,
        &eventHandler,
        this,
        NULL
    ));
    start(adapter);
}

void NetworkController::start(const NetworkAdapter& adapter)
{
    active_interface_ = interfaces_[static_cast<uint8_t>(adapter)];
    active_interface_->start();
}

void NetworkController::handleEvent(esp_event_base_t event, int32_t id, void* data)
{
    if (event == IP_EVENT && id == IP_EVENT_STA_GOT_IP) {
        ip_event_got_ip_t* event = (ip_event_got_ip_t*) data;
        ESP_LOGI(TAG, "got ip:" IPSTR, IP2STR(&event->ip_info.ip));
        return;
    }
    active_interface_->handleEvent(*this, id, data);
}

void NetworkController::setSsid(const char* ssid, const char* pass)
{
    strcpy((char *)sta_i_.wifi_config_.sta.ssid, ssid);
    strcpy((char *)sta_i_.wifi_config_.sta.password, pass);
    sta_i_.config_pending_ = true;
}

void WifiStaI::init()
{
    esp_wifi_set_mode(WIFI_MODE_STA);
    ESP_ERROR_CHECK(esp_wifi_get_config(WIFI_IF_STA, &wifi_config_));
    interface_ = esp_netif_create_default_wifi_sta();
    ESP_LOGI(TAG, "STA init ok");
    ESP_LOGI(TAG, "connecting with: %s  | %s", wifi_config_.sta.ssid, wifi_config_.sta.password);
}

void WifiApI::init()
{
    esp_wifi_set_mode(WIFI_MODE_AP);
    ESP_ERROR_CHECK(esp_wifi_get_config(WIFI_IF_AP, &wifi_config_));
    interface_ = esp_netif_create_default_wifi_ap();
    wifi_config_.ap.authmode = WIFI_AUTH_OPEN;
    /* FIXME: Don't always overwrite the config */
    std::string ssid = "ginco_bridge";
    sprintf((char *)wifi_config_.ap.ssid, ssid.data());
    wifi_config_.ap.ssid_len = ssid.length();
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_AP, &wifi_config_));
    ESP_LOGI(TAG, "WiFi AP init ok");
}

void WifiStaI::handleEvent(NetworkController& context, int32_t id, void* data)
{
    ESP_LOGI(TAG, "%s event %lu", name(), id);
    switch (id)
    {
    case WIFI_EVENT_STA_START:
    {
        if (esp_wifi_connect() == ESP_ERR_WIFI_SSID)
        {
            context.start(NetworkAdapter::AP);
        }
        break;
    }
    case WIFI_EVENT_STA_CONNECTED:
    {
        if (context.event_handler != nullptr)
        {
            context.event_handler(NetworkEvent::CONNECTED);
        }
        break;
    }
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

void WifiApI::handleEvent(NetworkController& context, int32_t id, void* data)
{
    ESP_LOGI(TAG, "%s event %lu", name(), id);
    switch (id)
    {
    case WIFI_EVENT_AP_START:
        driver::socket::init();
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

void WifiStaI::start()
{
    esp_wifi_stop();
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    if (config_pending_)
    {
        ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config_));
        config_pending_ = false;
    }
    ESP_ERROR_CHECK(esp_wifi_start());
    ESP_LOGI(TAG, "%s started.", name());
}

void WifiApI::start()
{
    esp_wifi_stop();
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_AP));
    if (config_pending_)
    {
        ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_AP, &wifi_config_));
        config_pending_ = false;
    }
    ESP_ERROR_CHECK(esp_wifi_start());
    ESP_LOGI(TAG, "%s started.", name());
}

static void eventHandler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data)
{
    assert(arg != nullptr);
    NetworkController* controller = reinterpret_cast<NetworkController*>(arg);
    controller->handleEvent(event_base, event_id, event_data);
}

void NetworkController::setHandler(std::function<void(NetworkEvent)> cb_fnc)
{
    event_handler = cb_fnc;
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
