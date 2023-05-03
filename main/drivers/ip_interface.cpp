#include "ip_interface.h"

#include "tcpip_adapter.h"
#include "freertos/event_groups.h"
#include "esp_system.h"

static const char *TAG = "Ip interface";
static EventGroupHandle_t eventGroup;
static IpDriverCallback IpDrivercb = nullptr;
static IpInterface* activeHandler;

using namespace ip_interface;

static esp_err_t event_handler(void *ctx, system_event_t *event) {
    if not (activeHandler != nullptr and event != nullptr)
        return ESP_OK;
    activeHandler->handle(event);
    activeHandler->setLastError(event->event_id);
    return ESP_OK;
}

bool init(IpDriverCallback cb){
    bool result {false};
    tcpip_adapter_init();
    eventGroup = xEventGroupCreate();
    result = esp_event_loop_init(event_handler, NULL) == ESP_OK;
    if (!result){
        ESP_LOGE(TAG, "Failed to start event loop.");
        return false;
    }

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    result = esp_wifi_init(&cfg) == ESP_OK;
    if (result)
        IpDrivercb = cb;
    ESP_LOGI(TAG, "initialisation response: %d", result);
    return result;
}

class WiFiApInterface : public IpInterface {
    static constexpr adapter_t type = adapter_t::ADAPTER_AP;
    public:
        virtual void start() {
            resetConnectionBit();
            wifi_config_t stationCfg = {};

        }
}