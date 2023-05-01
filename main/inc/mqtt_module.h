#ifndef _MY_MQTT_HEADER_H_
#define _MY_MQTT_HEADER_H_

#include "mqtt_client.h"

#define MQTT_KEY "MQTT_URL"
#define WIFI_KEY "SSID"
#define WIFI_PASS "WIFI_PASSWORD"

namespace mqtt {
    void init();
    void event_handler(void *handler_args, esp_event_base_t base,int32_t event_id, void *event_data);
    void send_message(const char *data);
    bool connected(void);
}

#endif