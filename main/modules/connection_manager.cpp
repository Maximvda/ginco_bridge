#include "connection_manager.h"

#include <stdio.h>
#include <atomic>
#include "string.h"

#include "esp_log.h"

#include "config_driver.h"
#include "ip_interface.h"
#include "event_list.h"
#include "socket_listener.h"
#include "mqtt_module.h"

static const char *TAG = "Con manager";

using namespace ip_interface;

static uint16_t retry_count{0};

static void event_callback(SignaList event, eAdapterType type)
{
    ESP_LOGI(TAG, "Got event callback %i", event);
    switch (event)
    {
    case SIGNAL_IPDRIVER_CONNECTED:
        if (type == eAdapterType::ADAPTER_STA)
        {
            if (!mqtt::init())
            {
                connection_manager::start(eAdapterType::ADAPTER_AP);
            }
        }
        break;
    case SIGNAL_IPDRIVER_RUNNING:
    {
        if (type == eAdapterType::ADAPTER_AP)
        {
            socket_listener::init();
        }
        break;
    }
    case SIGNAL_IPDRIVER_CONNECTION_ERROR:
        ip_interface::start(eAdapterType::ADAPTER_AP);
        break;
    default:
        break;
    }
}

void connection_manager::start(eAdapterType type)
{
    ip_interface::start(type);
}
